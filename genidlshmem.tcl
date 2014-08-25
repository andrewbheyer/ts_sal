#!/usr/bin/tclsh

#
# Step 1 : Modify datastreams.detail as necessary (web tools)
#      1a: or cat baseline_sep08.detail */*.detail > datastreams.detail
# Step 2 : ./genidlshmem.tcl to .idl and .sqldef files per topic
# Step 3 : ./genshmem.tcl [subsys] to generate c libraries/apps
# Step 4 : ./genshmtcl.tcl to generate tcl test code
# Step 5 : make ; touch Shared/*.o ; make to lib libshm.so
#

proc olddoitem { fid fo2 name type } {
global ITEMLIST
   switch $type {
       string -
       String { puts $fid "  string<32> $name;" 
                puts $fo2 "  $name char(32),"
                set ITEMLIST "$ITEMLIST,char.$name"
              }
       int    -
       long   -
       Int    { puts $fid "  long $name;" 
                puts $fo2 "  $name int,"
                set ITEMLIST "$ITEMLIST,int.$name"
              }
       short  -
       Short  { puts $fid "  short $name;" 
                puts $fo2 "  $name smallint,"
                set ITEMLIST "$ITEMLIST,short.$name"
              }
       char   -
       Char   {
                puts $fid "  string<1> $name;"
                puts $fo2 "  $name tinyint,"
                set ITEMLIST "$ITEMLIST,byte.$name"
              }
       byte   -
       Byte   { puts $fid "  octet $name;"
                puts $fo2 "  $name tinyint,"
                set ITEMLIST "$ITEMLIST,byte.$name"
              }
       float  -
       Float  { puts $fid "  float $name;" 
                puts $fo2 "  $name float," 
                 set ITEMLIST "$ITEMLIST,float.$name"
              }
       double -
       Double { puts $fid "  double $name;" 
                puts $fo2 "  $name double," 
                set ITEMLIST "$ITEMLIST,double.$name"
               }
   }
}
proc nditem { fid fo2 name type nd } {
global ITEMLIST
   switch $type {
       string -
       String { puts $fid "  string<$nd> $name;" 
                puts $fo2 "  $name char($nd),"
                set ITEMLIST "$ITEMLIST,char.$name"
              }
       int    -
       long   -
       Int    { puts $fid "  long $name\[$nd\];" 
                set i 1 ; while { $i <= $nd } {puts $fo2 "  [set name]_$i int," ; set ITEMLIST "$ITEMLIST,int.[set name]_$i"; incr i 1}
              }
       short  -
       Short  { puts $fid "  short $name\[$nd\];" 
                set i 1 ; while { $i <= $nd } {puts $fo2 "  [set name]_$i smallint,"; set ITEMLIST "$ITEMLIST,short.[set name]_$i" ; incr i 1}
              }
       char   -
       Char   {
                puts $fid "  string<$nd> $name;"
                set i 1 ; while { $i <= $nd } {puts $fo2 "  [set name]_$i tinyint,"; set ITEMLIST "$ITEMLIST,byte.[set name]_$i" ; incr i 1}
              }       byte   -
       Byte   { puts $fid "  octet $name\[$nd\];"
                set i 1 ; while { $i <= $nd } {puts $fo2 "  [set name]_$i tinyint,"; set ITEMLIST "$ITEMLIST,byte.[set name]_$i" ; incr i 1}
              }
       float  -
       Float  { puts $fid "  float $name\[$nd\];" 
                set i 1 ; while { $i <= $nd } {puts $fo2 "  [set name]_$i float,"; set ITEMLIST "$ITEMLIST,float.[set name]_$i"  ; incr i 1}
               }
       double -
       Double { puts $fid "  double $name\[$nd\];" 
                set i 1 ; while { $i <= $nd } {puts $fo2 "  [set name]_$i double," ; set ITEMLIST "$ITEMLIST,double.[set name]_$i" ; incr i 1}
               }
   }
}

source $env(SAL_DIR)/streamutils.tcl
set defdir .salwork
set last ""
set hasindex 0
set ITEMLIST ""
set fin [open $defdir/datastreams.detail r]
set fsum [open $defdir/datastreams.names w]
while { [gets $fin rec] > -1 } {
   if { [string range $rec 0 5] == "#index" } {
      set ki [lindex $rec 1]
      gets $fin rec
      set d [split [lindex $rec 0] "./_"]
      set ncmp [expr [llength $d] -2]
      if { $ncmp < 0}  {set ncmp 0}
      set id [join [lrange $d 0 $ncmp] _]
      set keyindex($id) $ki
   }
   set id [lindex $rec 0]
   set topic [lindex $rec 0]
   if { $id != $last  } {
      if { $last != "" } {puts $fsum $last}
      catch { puts $fout "\};"
              close $fout
              if { [info exists keyindex($last)] } {
                puts $fo2 "  PRIMARY KEY (date_time,$keyindex($last))"
              } else {
                puts $fo2 "  PRIMARY KEY (date_time)"
              }
              puts $fo2 ");"
              close $fo2
              puts $fo3 "set SQLREC($last) \"$ITEMLIST\""
              close $fo3
            }
      set last $id
      set fout [open $id.idl w]
      set fo2  [open $id.sqldef w]
      set fo3  [open $id.sqlwrt w]
      puts $fout "struct $topic \{
  string<32> private_revCode; //private
  double private_sndStamp; //private
  double private_rcvStamp; //private
  long private_seqNum; //private
  long private_origin; //private"
      puts $fo2  "DROP TABLE IF EXISTS $topic;"
      puts $fo2  "CREATE TABLE $topic ("
      puts $fo2  "  date_time datetime NOT NULL,
  private_revCode char(32),
  private_sndStamp double,
  private_rcvStamp double,
  private_seqNum int,
  private_origin int,"
      set ITEMLIST "char.revCode,double.sndStamp,double.rcvStamp,int.seqNum,int.origin"
   }
   set name [lindex [split [lindex $rec 1] .] end]
   set type [lindex $rec 3]
   if { [lindex $rec 2] > 1 } {
      set nd [lindex $rec 2]
#puts stdout "nditem $fout $fo2 $name $type $nd"
      nditem $fout $fo2 $name $type $nd
   } else {
#puts stdout "olddoitem $fout $fo2 $name $type"
      olddoitem $fout $fo2 $name $type
   }
   puts stdout "Added $name to $topic"
}

puts $fout "\};"
close $fout
if { [info exists keyindex($last)] } {
 puts $fo2 "  PRIMARY KEY (date_time,$keyindex($last))"
} else {
  puts $fo2 "  PRIMARY KEY (date_time)"
}
puts $fo2 ");"
close $fo2
puts $fo3 "set SQLREC($last) \"$ITEMLIST\""
close $fo3
           



close $fsum
set lidl [glob $SAL_WORK_DIR/idl-templates/validated/*.idl]
set fmd5 [open .salwork/revCodes.tcl w]
foreach i [lsort $lidl] {
  set c [lindex [exec md5sum $i] 0]
  set s [file tail [file rootname $i]]
  puts $fmd5 "set REVCODE($s) $c"
}
close $fmd5


