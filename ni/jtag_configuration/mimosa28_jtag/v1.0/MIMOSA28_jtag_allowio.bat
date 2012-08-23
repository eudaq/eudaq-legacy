copy .\jtag_allowio_ms_dll.dll .\jtag_ms_dll.dll
del /F .\test_tmp\*.txt
allowio.exe Mi28.exe p:0x378 /a