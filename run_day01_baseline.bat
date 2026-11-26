@echo off
setlocal
cd /d "E:\c++project\GeoBase"

echo ============================================
echo  Day01 - capture BEFORE baseline
echo  Run this BEFORE changing any source code.
echo ============================================
echo.

if not exist "docs\baseline" mkdir "docs\baseline"
if not exist "docs\numeric"  mkdir "docs\numeric"

rem logMetric appends to CSV. Back up the old one first so the baseline stays clean.
if exist "docs\baseline\day01_before.csv" (
    move /Y "docs\baseline\day01_before.csv" "docs\baseline\day01_before_old.csv" >nul
    echo [1] Old CSV backed up to day01_before_old.csv
) else (
    echo [1] No old CSV found, nothing to back up
)

set "GEABASE_NUMERIC_CSV=E:\c++project\GeoBase\docs\baseline\day01_before.csv"
echo [2] Running full test suite (expect 2 PASS / 9 FAIL)...
echo.

x64\Debug\GeoBase.exe --gtest_output=xml:docs\baseline\day01_before.xml

echo.
echo ============================================
echo  Finished. Exit code = %ERRORLEVEL%
echo  Exit code 1 is EXPECTED: 9 tests are red
echo  before the three bugs are fixed.
echo ============================================
echo.
echo Files now in docs\baseline:
dir /b "docs\baseline\day01_before.*"
echo.
echo Next: read the red output, write 2026-11-17_baseline.md,
echo       then start fixing Matrix4 / Bezier.
echo.
pause
