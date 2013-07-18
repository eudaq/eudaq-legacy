Add-Type @'
using System;
using System.Runtime.InteropServices;

 public class Win32 {
 

 
[DllImport("user32.dll")]
[return: MarshalAs(UnmanagedType.Bool)]
public static extern bool MoveWindow(IntPtr hWnd, int X, int Y, int nWidth, int nHeight, bool bRepaint);
}
'@
echo “Start Eudaq“
# starting the Przesses
 Start-Process .\TestRunControl.exe
 start-sleep -s 1
 Start-Process .\TestLogCollector.exe -r tcp://127.0.0.1:44000
 start-sleep -s 1
 Start-Process .\TestDataCollector.exe -r tcp://127.0.0.1:44000
 start-sleep -s 1
 #rezisizing the windows
  $h=Get-Process | Where { $_.Name -Eq "testruncontrol" }
 [Win32]::MoveWindow($h.MainWindowHandle,0,0,700,800,1)
  $h1=Get-Process | Where { $_.Name -Eq "TestLogCollector" }
 [Win32]::MoveWindow($h1.MainWindowHandle,0,780,1000,300,1)
   $h2=Get-Process | Where { $_.Name -Eq "TestDataCollector" }
 [Win32]::MoveWindow($h2.MainWindowHandle,720,0,1000,500,1)
 # monitoring the main Prozess
do{

start-sleep -s 5 #to not waste to much cpu it is 5 secounds 
$process1 = Get-Process | Where-Object {$_.ProcessName -eq "testruncontrol"}
$process2 = Get-Process | Where-Object {$_.ProcessName -eq "TestDataCollector"}
$process3 = Get-Process | Where-Object {$_.ProcessName -eq "TestLogCollector"}
}while($process1 -and $process2 -and $process3)
start-sleep -s 1
Get-Process | Where { $_.Name -Eq "TestDataCollector" } | Kill
Get-Process | Where { $_.Name -Eq "TestLogCollector" } | Kill
Get-Process | Where { $_.Name -Eq "TestRunControl" } | Kill

echo “End“