#!/bin/sh
# the next line restarts using wish \
exec wish "$0" "$@"

# hello --
# Simple Tk script to create a button that prints "Hello, world".
# Click on the button to terminate the program.
#
# RCS: @(#) $Id: hello,v 1.4 2003/09/30 14:54:30 dkf Exp $

package require Tk
package require registry
package require tcom


proc sleep {N} {
	update idletasks
   after [expr {int($N * 1000)}]
   update idletasks
}



# set x [expr {[winfo screenwidth .]/4}]
# set y [expr {[winfo screenheight .]/4}]
set x 600
set y 700
wm geometry . ${x}x${y} 
wm title  . "INTERFACING MI28 BY COM"
frame .top -height 700 -width 600
place .top -x 0 -y 0

text .top.text -yscrollcommand [list .top.scroll set] -setgrid 0 \
-undo 1 
scrollbar .top.scroll -command [list .top.text yview] -width 20 
place .top.scroll -x 580 -y 0 -height 650
place .top.text -x 0 -y 0  -width 580 -height 650
 
# The first line below creates the button, and the second line
# asks the packer to shrink-wrap the application's main window
# around the button.

set progId_List { }

button .top.getServices -text "Get Services" -width  20 -height 1 -command {	
	set classesRootKey "HKEY_CLASSES_ROOT\\CLSID"
	foreach clsid [registry keys $classesRootKey] {
		if { $clsid == "{73C2EA3C-0A8A-4700-B710-F19EB440DCC7}" } {
		set clsidKey "$classesRootKey\\$clsid"
		set progIdSubKey [registry keys $clsidKey "VersionIndependentProgID"]
		if {[llength $progIdSubKey] > 0} {
			set progId [registry get "$clsidKey\\$progIdSubKey" ""]
			# puts stdout $progId
			.top.text insert end  $clsidKey 
			.top.text insert end  " - " 
			.top.text insert end  $progIdSubKey 
			.top.text insert end  " - " 
			.top.text insert end  $progId
			.top.text insert end  "\n"
			lappend progId_List $progId  
		} else {
			.top.text insert end  $clsidKey 
			.top.text insert end  " - " 
			.top.text insert end  $progIdSubKey 
			.top.text insert end  " - " 
			.top.text insert end  "NO  progIdSubKey"
			.top.text insert end  "\n"			
		
		}
		}
	}
}
place .top.getServices -x 30 -y 660


button .top.quit -text "Quit" -width  10 -height 1 -command { 
    ::tcom::unbind $MI28MasterConf
	destroy .
} 
place  .top.quit	-x 200 -y 660


button .top.clear -text "Clear" -width  10 -height 1 -command { .top.text delete 0.0 end}
place .top.clear	-x 300 -y 660

button .top.getMI28MasterConf -text "Get MI28MasterConf" -width  25 -height 1 -command {

set MI28MasterConf [::tcom::ref createobject "MI28JTAG.MI28COM"]

update

set MI28MasterConfInfo [$MI28MasterConf Info]
.top.text insert end $MI28MasterConfInfo 
.top.text insert end "\n"

update
sleep 1

.top.text insert end "\nTEST check reset of device \n"

set MI28MasterConfInfo [$MI28MasterConf MasterConfReset]
.top.text insert end $MI28MasterConfInfo  
.top.text insert end "\n"

update 
sleep 5

.top.text insert end "\nTEST check Load master config file \n"

set MI28MasterConfInfo [$MI28MasterConf MasterConfLoadFile "C:\\CCMOS_SCTRL\\MIMOSA28_JTAG\\config_files\\MI28_DEF_TEMPLATE2.mcf"]
.top.text insert end $MI28MasterConfInfo 
.top.text insert end "\n"

update 
sleep 1


.top.text insert end "\nTEST check Update parameters  \n"

set MI28MasterConfInfo [$MI28MasterConf MasterConfUpdateAll]
.top.text insert end $MI28MasterConfInfo 
.top.text insert end "\n"

update 
sleep 1


set MI28MasterConfInfo [$MI28MasterConf MasterConfUpdateBias]
.top.text insert end $MI28MasterConfInfo 
.top.text insert end "\n"

update 
sleep 1


set MI28MasterConfInfo [$MI28MasterConf MasterConfUpdateLinePat]
.top.text insert end $MI28MasterConfInfo 
.top.text insert end "\n"

update 
sleep 1


.top.text insert end "\nTEST check readback  parameters  \n"

set err "n"
set MI28MasterConfInfo [$MI28MasterConf MasterConfReadBack err]
.top.text insert end $MI28MasterConfInfo "err: " $err 
.top.text insert end "\n"

update 
sleep 1


.top.text insert end "\nTEST check Start of device  \n"

set MI28MasterConfInfo [$MI28MasterConf MasterConfStart]
.top.text insert end $MI28MasterConfInfo  
.top.text insert end "\n"

update 
sleep 1


.top.text insert end "\nTEST checkSet parallel port address  \n"


set paddr "888"
set rpaddr { }
set MI28MasterConfInfo [$MI28MasterConf MasterConfSetParallelPortAddr  $paddr  rpaddr]
set msg [format "%s    paddr:0x%x  rb_paddr: 0x%x" $MI28MasterConfInfo $paddr  $rpaddr] 
.top.text insert end $msg 
.top.text insert end "\n"

update 
sleep 2

.top.text insert end "\nTEST check Save master config file \n"

set MI28MasterConfInfo [$MI28MasterConf MasterConfSaveAllFile]
.top.text insert end $MI28MasterConfInfo  
.top.text insert end "\n"

update 
sleep 5


.top.text insert end "\nTEST check change of device \n"


set MI28MasterConfInfo [$MI28MasterConf MasterConfSetDevNum 0]
.top.text insert end $MI28MasterConfInfo  
.top.text insert end "\n"

update 
sleep 1


set rdev { }
set MI28MasterConfInfo [$MI28MasterConf MasterConfGetDevNum rdev]
.top.text insert end $MI28MasterConfInfo " " $rdev
.top.text insert end "\n"

update 
sleep 1

set MI28MasterConfInfo [$MI28MasterConf MasterConfSetDevNum 1]
.top.text insert end $MI28MasterConfInfo  
.top.text insert end "\n"

update 
sleep 1


set rdev { }
set MI28MasterConfInfo [$MI28MasterConf MasterConfGetDevNum rdev]
.top.text insert end $MI28MasterConfInfo " " $rdev
.top.text insert end "\n"

update 
sleep 2


set MI28MasterConfInfo [$MI28MasterConf MasterConfSetDevNum 0]
.top.text insert end $MI28MasterConfInfo  
.top.text insert end "\n"

update 
sleep 1


set rdev { }
set MI28MasterConfInfo [$MI28MasterConf MasterConfGetDevNum rdev]
.top.text insert end $MI28MasterConfInfo " " $rdev
.top.text insert end "\n"

update 
sleep 1

.top.text insert end "\nTEST check change of DAC registers \n"

for {set i 0} {$i<19} {incr i}  { 
	set MI28MasterConfInfo [$MI28MasterConf Mimosa28ConfSetBias $i $i]
	# .top.text insert end $MI28MasterConfInfo  
	# .top.text insert end "\n"

	update 
}

sleep 1

for {set i 0} {$i<19} {incr i}  {
	set readback { }
	set rb_readback { }
	set MI28MasterConfInfo [$MI28MasterConf Mimosa28ConfGetBias $i  readback rb_readback]
	set msg [format "%s  i:%d  w:%d r: %d" $MI28MasterConfInfo  $i $readback $rb_readback] 
	.top.text insert end $msg 
	.top.text insert end "\n"

	update 
}

sleep 1


.top.text insert end "\nTEST check change of ROMODE registers \n"

for {set i 0} {$i<40} {incr i}  { 
	set MI28MasterConfInfo [$MI28MasterConf Mimosa28ConfSetRoMode $i 0]
	#  .top.text insert end $MI28MasterConfInfo  
	#  .top.text insert end "\n"
	
	update 
}


for {set i 0} {$i<40} {incr i}  { 
	set MI28MasterConfInfo [$MI28MasterConf Mimosa28ConfSetRoMode $i 1]
	# .top.text insert end $MI28MasterConfInfo  
	# .top.text insert end "\n"
	
	update 
}



update 
sleep 1

set MI28MasterConfInfo [$MI28MasterConf -method Mimosa28ConfGetRoMode 0 readback ]

for {set i 0} {$i<40} {incr i}  { 
	set MI28MasterConfInfo [$MI28MasterConf -method Mimosa28ConfGetRoMode 0 readback ]
	set msg [format "%s  i:%d  w:%d" $MI28MasterConfInfo  $i $readback] 
	.top.text insert end $msg 
	.top.text insert end "\n"	
	
}
	
update 
sleep 1

.top.text insert end "\nTEST check change of DISCRI registers \n"


for {set i 0} {$i<30} {incr i}  { 
	set MI28MasterConfInfo [$MI28MasterConf Mimosa28ConfSetDiscri $i 0]
	update 
}


set value  [expr 0xFFFFFFFF * 1]
for {set i 0} {$i<30} {incr i}  { 
	set val [expr [expr $value & 0xFFFFFF00] + $i] 
	set MI28MasterConfInfo [$MI28MasterConf Mimosa28ConfSetDiscri $i $val ]

	update 
}




update 
sleep 1


for {set i 0} {$i<30} {incr i}  { 
	set readback { }
	set rb_readback { }
	set MI28MasterConfInfo [$MI28MasterConf Mimosa28ConfGetDiscri $i readback rb_readback ]
	set msg [format "%s  i:%d  w:0x%x  r:0x%x" $MI28MasterConfInfo  $i $readback $rb_readback] 
	.top.text insert end $msg 
	.top.text insert end "\n"
}


update 
sleep 1

.top.text insert end "\nTEST check change of LINEPAT0 registers \n"

for {set i 0} {$i<30} {incr i}  { 
	set MI28MasterConfInfo [$MI28MasterConf Mimosa28ConfSetLinePat0 $i 0]
	update 
} 
sleep 1

set value  [expr 0xAAAAAAAA * 1]

for {set i 0} {$i<30} {incr i}  { 
    set val [expr [expr $value & 0xFFFFFF00] + $i] 
	set MI28MasterConfInfo [$MI28MasterConf Mimosa28ConfSetLinePat0 $i $val ]
	update 
} 


for {set i 0} {$i<30} {incr i}  { 
	set readback { }
	set rb_readback { }
	set MI28MasterConfInfo [$MI28MasterConf Mimosa28ConfGetLinePat0 $i readback rb_readback]
	set msg [format "%s  i:%d  w:0x%x  r:0x%x" $MI28MasterConfInfo  $i $readback $rb_readback] 
	.top.text insert end $msg 
	.top.text insert end "\n"

}

update 
sleep 1

.top.text insert end "\nTEST check change of LINEPAT1 registers \n"

set value  [expr 0xBBBBBBBB * 1]

for {set i 0} {$i<30} {incr i}  { 
	set val [expr [expr $value & 0xFFFFFF00] + $i] 
	set MI28MasterConfInfo [$MI28MasterConf Mimosa28ConfSetLinePat1 $i $val ]
	update 
} 

update 
sleep 1

for {set i 0} {$i<30} {incr i}  { 
	set readback { }
	set rb_readback { }
	set MI28MasterConfInfo [$MI28MasterConf Mimosa28ConfGetLinePat1 $i readback rb_readback]
	set msg [format "%s  i:%d w:0x%x  r:0x%x" $MI28MasterConfInfo  $i $readback $rb_readback] 
	.top.text insert end $msg 
	.top.text insert end "\n"
}

update 
sleep 1
.top.text insert end "\nTEST check change of SEQPIX registers \n"


for {set i 0} {$i<7} {incr i}  { 
	set MI28MasterConfInfo [$MI28MasterConf Mimosa28ConfSetSeqPix $i $i]
}

update 
sleep 1

for {set i 0} {$i<7} {incr i}  { 
	set readback { }
	set rb_readback { }
	set MI28MasterConfInfo [$MI28MasterConf Mimosa28ConfGetSeqPix $i readback rb_readback]
	set msg [format "%s  i:%d  w:0x%x  r:0x%x" $MI28MasterConfInfo  $i $readback $rb_readback] 
	.top.text insert end $msg 
	.top.text insert end "\n"
}
	
update 
sleep 3

for {set i 0} {$i<10} {incr i}  { 
	set MI28MasterConfInfo [$MI28MasterConf Mimosa28ConfSetSeqSuze $i $i]
}

update 
sleep 1

for {set i 0} {$i<10} {incr i}  { 
	set readback { }
	set rb_readback { }
	set MI28MasterConfInfo [$MI28MasterConf Mimosa28ConfGetSeqSuze $i readback rb_readback]
	set msg [format "%s  i:%d  w:0x%x  r:0x%x" $MI28MasterConfInfo  $i $readback $rb_readback] 
	.top.text insert end $msg 
	.top.text insert end "\n"
}

update 
sleep 1

set value  [expr 0x5555 * 1]
for {set i 0} {$i<4} {incr i}  { 
	set MI28MasterConfInfo [$MI28MasterConf Mimosa28ConfSetHeaderTrailer $i $value]
}

update 
sleep 1

for {set i 0} {$i<4} {incr i}  { 
	set readback { }
	set rb_readback { }
	set MI28MasterConfInfo [$MI28MasterConf Mimosa28ConfGetHeaderTrailer $i  readback rb_readback]
	set msg [format "%s  i:%d  w:0x%x  r:0x%x" $MI28MasterConfInfo  $i $readback $rb_readback] 
	.top.text insert end $msg 
	.top.text insert end "\n"
}

update 
sleep 1

for {set i 0} {$i<3} {incr i}  { 
	set MI28MasterConfInfo [$MI28MasterConf Mimosa28ConfSetMon1 $i $i]
}

update 
sleep 1


for {set i 0} {$i<3} {incr i}  { 
	set readback { }
	set rb_readback { }
	set MI28MasterConfInfo [$MI28MasterConf Mimosa28ConfGetMon1 $i  readback rb_readback]
	set msg [format "%s  i:%d  w:%d r: %d" $MI28MasterConfInfo  $i $readback $rb_readback] 
	.top.text insert end $msg 
	.top.text insert end "\n"
}

update 
sleep 1

for {set i 0} {$i<2} {incr i}  { 
	set MI28MasterConfInfo [$MI28MasterConf Mimosa28ConfSetMon2 $i  $i]
}

update 
sleep 1

for {set i 0} {$i<2} {incr i}  { 
	set readback { }
	set MI28MasterConfInfo [$MI28MasterConf Mimosa28ConfGetMon2 $i  readback]
	set msg [format "%s  i:%d  w:%d" $MI28MasterConfInfo  $i $readback] 
	.top.text insert end $msg 
	.top.text insert end "\n"	
}


update 
sleep 3



set MI28MasterConfInfo [$MI28MasterConf Mimosa28ConfShowWindow 0]
.top.text insert end $MI28MasterConfInfo  
.top.text insert end "\n"

update 
sleep 2
set MI28MasterConfInfo [$MI28MasterConf Mimosa28ConfShowWindow 1]
.top.text insert end $MI28MasterConfInfo  
.top.text insert end "\n"

update 
sleep 3

set MI28MasterConfInfo [$MI28MasterConf Mimosa28MessageShowWindow 0]
.top.text insert end $MI28MasterConfInfo  
.top.text insert end "\n"

update 
sleep 2

set MI28MasterConfInfo [$MI28MasterConf Mimosa28MessageShowWindow 1]
.top.text insert end $MI28MasterConfInfo  
.top.text insert end "\n"


update 
sleep 3


set eval_time_start [clock milliseconds] 
set MI28MasterConfInfo2 [$MI28MasterConf  MasterConfReset ]
 
 
    if { 1 < 0 } {
	for {set x 0} {$x<255} {incr x} {
		set MI28MasterConfInfo3 [$MI28MasterConf -method   Mimosa28ConfSetBias  0 $x]
		.top.text insert end $MI28MasterConfInfo3 
		.top.text insert end $x 
		.top.text insert end "\n"
		
		set MI28MasterConfInfo3 [$MI28MasterConf -method  MasterConfStart]
		.top.text insert end $MI28MasterConfInfo3 
		.top.text insert end "\n"
		set MI28MasterConfInfo3 [$MI28MasterConf  -method  MasterConfUpdateBias]
		.top.text insert end $MI28MasterConfInfo3 
		.top.text insert end "\n"
		
		set MI28MasterConfInfo3 [$MI28MasterConf  -method  MasterConfReadBack]
		.top.text insert end $MI28MasterConfInfo3 
		.top.text insert end "\n"
		
		set MI28MasterConfInfo3 [$MI28MasterConf -method   Mimosa28ConfGetBias  0 readback readback_rb]
		.top.text insert end $MI28MasterConfInfo3 
		.top.text insert end " readback "  
		.top.text insert end $readback	
		.top.text insert end " readback_rb "  
		.top.text insert end  $readback_rb		
		.top.text insert end "\n"
		update 
	}
}
set eval_time_stop [clock milliseconds]
	
.top.text insert end "execution time :"
.top.text insert end [expr $eval_time_stop - $eval_time_start]
.top.text insert end "ms \n"

}
place .top.getMI28MasterConf	-x 400 -y 660



