$ErrorActionPreference = "Stop"

$ProjectRoot = Split-Path -Parent $PSScriptRoot
$UProject = Join-Path $ProjectRoot "SpatialMathLab.uproject"
$EditorCmd = "C:\Program Files\Epic Games\UE_5.8\Engine\Binaries\Win64\UnrealEditor-Cmd.exe"

$Timestamp = Get-Date -Format "yyyyMMdd_HHmmss"
$ReportRoot = Join-Path $ProjectRoot "Saved\AutomationReports\SpatialMathCore_$Timestamp"
$LogPath = Join-Path $ReportRoot "SpatialMathCore.log"

function Fail {
    param([string]$Message)

    Write-Host ""
    Write-Host "ERROR: $Message" -ForegroundColor Red
    exit 1
}

foreach ($RequiredPath in @(
    $UProject,
    $EditorCmd
)) {
    if (-not (Test-Path -LiteralPath $RequiredPath)) {
        Fail "Required path was not found: $RequiredPath"
    }
}

New-Item -ItemType Directory -Path $ReportRoot -Force | Out-Null

foreach ($ProcessName in @(
    "UnrealEditor",
    "UnrealEditor-Cmd",
    "LiveCodingConsole"
)) {
    Get-Process `
        -Name $ProcessName `
        -ErrorAction SilentlyContinue |
    Stop-Process `
        -Force `
        -ErrorAction SilentlyContinue
}

$Arguments = @(
    "`"$UProject`"",
    "-unattended",
    "-nop4",
    "-NullRHI",
    "-NoSound",
    "-NoSplash",
    "-stdout",
    "-FullStdOutLogOutput",
    "-UTF8Output",
    "-abslog=`"$LogPath`"",
    "-ReportExportPath=`"$ReportRoot`"",
    "-ExecCmds=`"Automation RunTest SpatialMathCore;Quit`""
)

$Info = New-Object System.Diagnostics.ProcessStartInfo
$Info.FileName = $EditorCmd
$Info.Arguments = ($Arguments -join " ")
$Info.WorkingDirectory = $ProjectRoot
$Info.UseShellExecute = $false
$Info.RedirectStandardOutput = $true
$Info.RedirectStandardError = $true
$Info.CreateNoWindow = $true

$Process = New-Object System.Diagnostics.Process
$Process.StartInfo = $Info

$null = $Process.Start()

$OutputTask = $Process.StandardOutput.ReadToEndAsync()
$ErrorTask = $Process.StandardError.ReadToEndAsync()

$Completed = $Process.WaitForExit(300000)

if (-not $Completed) {
    try {
        $Process.Kill()
    }
    catch {
    }

    Fail "The test run exceeded five minutes."
}

$Output = $OutputTask.Result
$Errors = $ErrorTask.Result

if (-not (Test-Path -LiteralPath $LogPath)) {
    [System.IO.File]::WriteAllText(
        $LogPath,
        $Output + "`r`n" + $Errors,
        (New-Object System.Text.UTF8Encoding($false))
    )
}

$Combined =
    (Get-Content -LiteralPath $LogPath -Raw) +
    "`n" +
    $Output +
    "`n" +
    $Errors

$SuccessMarkers = (
    [regex]::Matches(
        $Combined,
        "Result=\{Success\}"
    )
).Count

$FailureMarkers = (
    [regex]::Matches(
        $Combined,
        (
            "Result=\{Fail\}|" +
            "Result=\{Error\}|" +
            "Automation Test Failed|" +
            "Test Failed"
        )
    )
).Count

if ($Process.ExitCode -ne 0 -or
    $FailureMarkers -gt 0 -or
    $SuccessMarkers -lt 25) {

    Fail @"
SpatialMathCore verification failed.

Exit code: $($Process.ExitCode)
Success markers: $SuccessMarkers
Failure markers: $FailureMarkers
Log: $LogPath
"@
}

Write-Host ""
Write-Host "ALL 25 SPATIAL MATH CORE TESTS PASSED" -ForegroundColor Green
Write-Host "Success markers: $SuccessMarkers" -ForegroundColor White
Write-Host "Report: $ReportRoot" -ForegroundColor White
Write-Host ""