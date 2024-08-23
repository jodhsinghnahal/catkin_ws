To compile this project in CC:

1. modify the macro.ini file in this folder to reflect your local workspace paths to the respective directories:
	Library path: location of rts2800_ml.lib (...\Software\Code\Common\Targets\TI\28x\Lib)
	Device root: location of all device specific files (...\Software\Code\28x)
	Common root: location of all common files (...\Software\Code\Common)
   and place this file in &(install_dir)\ti\cc\bin

2. start Code Composer

3. Load the workspace in this directory (TmrDrv.wks)

3a. re-compile the project if so desired.  By default, the gel file will load the .out file in the Bin directory if it's there.

4. The gel file should run when the project is loaded.  If it doesn't, reload (or load) the gel file.

5. Press F5 to run the test program, or select 'Run_Timer_Tests' from the GEL:Timer Driver Unit Tests menu.

6. Messages will appear in the gel message box indicating the status of the test(s).