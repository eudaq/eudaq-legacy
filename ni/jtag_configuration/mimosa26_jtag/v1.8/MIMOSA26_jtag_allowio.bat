copy .\jtag_allowio_ms_dll.dll .\jtag_ms_dll.dll
del /F .\test_tmp\*.txt
allowio.exe Mi26.exe p:0xdcf0 /a
Mi26.exe 