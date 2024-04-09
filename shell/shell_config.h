#ifndef _SHELL_CONFIG_H_
#define _SHELL_CONFIG_H_


//*** <<< Use Configuration Wizard in Context Menu >>> ***
	
// <h> External Bus Interface (EBI)

//   <e1.13> Enable Chip Select 0 (CSR0)
//     <o1.20..31> BA: Base Address <0x0-0xFFF00000:0x100000><#/0x100000>
//                 <i> Start Address for Chip Select Signal
//     <o1.7..8>   PAGES: Page Size      <0=> 1M Byte    <1=> 4M Bytes
//                                       <2=> 16M Bytes  <3=> 64M Bytes
//                 <i> Selects Active Bits in Base Address
//     <o1.0..1>   DBW: Data Bus Width   <1=> 16-bit     <2=> 8-bit
//     <o1.12>     BAT: Byte Access Type <0=> Byte-write
//                                       <1=> Byte-select
//     <e1.5>      WSE: Enable Wait State Generation
//       <o1.2..4>   NWS: Number of Standard Wait States <1-8><#-1>
//     </e>
//     <o1.9..11>  TDF: Data Float Output Time <0-7>
//                 <i> Number of Cycles Added after the Transfer
//   </e>
_WDWORD(0xFFE00000, 0x010024A9);   // EBI_CSR0: Flash

//   <e1.13> Enable Chip Select 1 (CSR1)
//     <o1.20..31> BA: Base Address <0x0-0xFFF00000:0x100000><#/0x100000>
//                 <i> Start Address for Chip Select Signal
//     <o1.7..8>   PAGES: Page Size      <0=> 1M Byte    <1=> 4M Bytes
//                                       <2=> 16M Bytes  <3=> 64M Bytes
//                 <i> Selects Active Bits in Base Address
//     <o1.0..1>   DBW: Data Bus Width   <1=> 16-bit     <2=> 8-bit
//     <o1.12>     BAT: Byte Access Type <0=> Byte-write
//                                       <1=> Byte-select
//     <e1.5>      WSE: Enable Wait State Generation
//       <o1.2..4>   NWS: Number of Standard Wait States <1-8><#-1>
//     </e>
//     <o1.9..11>  TDF: Data Float Output Time <0-7>
//                 <i> Number of Cycles Added after the Transfer
//   </e>
_WDWORD(0xFFE00004, 0x040034A5);   // EBI_CSR1: RAM

//   <q1.4>        DRP: Data Read Protocol
//                      <0=> Standard Read
//                      <1=> Early Read
_WDWORD(0xFFE00024, 0x00000010);   // EBI_MCR:  Data Read Protocol

_WDWORD(0xFFE00020, 0x00000001);   // EBI_RCR:  Remap Command

// </h>

// <o> Program Entry Point
PC = 0x04000000;

// <s> Change ID
// <s1.30> Change Password String
#define ID  "My User ID"
char pw[] = "My Password";


/*********************************************************/
/*  Example for enabling and disabling code              */

// <c1> Use MY_CPU_VARIANT
// <i> Use MY_CPU_VARIANT, and set the include file
#define MY_CPU_VARIANT
#include "MyCpuVariant.h"
// </c>

// <!c1> Disable log
// <i> Disable log file generation
#define _USE_LOG
// </c>

// <c1>  Example of inconsistent comment
// <i> a mix of commented and uncommented lines in the block create an inconsistency

// This type of comment, mixed with uncommented lines, creates the inconsistency
/* You can use this type of comment without creating an inconsistency */

do_whatever;              // adding this type of comment here is allowed

// </c>


//*** <<< end of configuration section >>>    ***

#endif