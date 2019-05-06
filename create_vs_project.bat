@rem Call cmake twice to update the configuration set (which needs a second run).
@set GENERATOR=call cmake "-H." "-Bbuild" "-GVisual Studio 16 2019"
@%GENERATOR%
@if %ErrorLevel% EQU 0 (
    @%GENERATOR%
) else (
    @echo "CMake error code: %ErrorLevel%"
    @pause
)
@exit /b %ErrorLevel%
