cls
@echo off
path = %~dp0Python27;%~dp0\Python27\Scripts;%~dp0\Python27\Lib\site-packages\django\bin;%~dp0\Utilities;%~dp0\Utilities\Exemaker;%~dp0\Utilities\Npp;%~dp0\Utilities\Sqlite;%PATH%
set PYTHONPATH=%~dp0%\Python27;%~dp0%\Python27\Lib\site-packages
exemaker -i "%~dp0\Python27\python.exe" "%~dp0\Python27\Lib\site-packages\django\bin\django-admin.py" "%~dp0\Python27\Lib\site-packages\django\bin" 1>nul 2>&1
echo.
echo Instant Django is now ready
echo.
cmd /k