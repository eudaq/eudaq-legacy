@echo off
if not "%VCINSTALLDIR%" == "" goto start
call "C:\Program Files\Microsoft Visual Studio 8\VC\vcvarsall.bat"

:start
link /def:ZestSC1.def /dll /out:ZestSC1.dll /machine:x86 ZestSC1.lib SetupAPI.lib
coff2omf ZestSC1_orig.lib ZestSC1_new.lib
