/* Default linker script, for normal executables */
OUTPUT_FORMAT("elf32-kungfu32-little", "elf32-kungfu32-big","elf32-kungfu32-little")
OUTPUT_ARCH(kungfu32)
ENTRY(_start)
SEARCH_DIR(".");

MEMORY
{ 
  flash      : ORIGIN =  0x00000000, LENGTH = 0x00038000
  ram        : ORIGIN =  0x10000000, LENGTH = 0x00020000
  da_mem     : ORIGIN =  0x0C001C00, LENGTH = 0x00000400
  mode_mem   : ORIGIN =  0x0C001800, LENGTH = 0x00000004
  pro_mem    : ORIGIN =  0x0C001000, LENGTH = 0x00000004
  ee_mem     : ORIGIN =  0x7F000000, LENGTH = 0x00001000
  config_mem1 : ORIGIN = 0x31000000, LENGTH = 0x00000010
  config_mem2 : ORIGIN = 0x31000010, LENGTH = 0x00000010
}

SECTIONS
{

  .text : 
  { 
    . = 0x0000;
    KEEP (*vector.o(.text*))   /* chip interrupt vector ,writed in file named vector.s or vector.c */

	. = (. + 3) & (-4);	/* align 4 */
    __vec_end__ = .;
    
    *(.text*)		     /* default function code space */	
    
     *(.ctors*)
     *(.dtors*)
    __init_class_start = (. + 3) & (-4);
    KEEP (*(.init_array*)) 	/* class init function list space */	
    __init_class_end = .;
     *(.init*)
     *(.fini*)
        
    *(.rdata*)		     /* global const variable space */
    *(.rodata*)		     /* auto const variable space */

	. = (. + 3) & (-4);
    *(.usertext*)	       /* reserved for custom */	
    *(.userrdata*)
    *(.userrodata*)

	. = (. + 3) & (-4);
    __text_end__ = .;		
				/*record: global variable value of initialization */	

  } > flash  

  .data :
  {
	. = (. + 3) & (-4);
  __data_start__ = .;

    KEEP (*(.ramvector*))    	/* default server for vector writed in ram */

  __lpdata_start__ = (. + 3) & (-4);
    *(.lpdata*)   		/* Reset but Keep Space  */
    /* . = 0x4000 ;  */         /* Max length limit is 16K byte ,if need 16K all config ,delete comment symbol*/
  __lpdata_end__ =  (. + 3) & (-4);

   	. = (. + 3) & (-4);
    *(.indata*)   		/* server space for ram function */
    *(.inrdata*)
    *(.inrodata*)

	. = (. + 3) & (-4);
    *(.data*) 	  		/* global variable with  initialization */
	. = (. + 3) & (-4);
  __data_end__ = .;

	. = (. + 3) & (-4);
  __bss_start__ = .;  		/* global variable with no initialization */
    *(COMMON) 	
    *(.bss)
	. = (. + 3) & (-4);
  __bss_end__ = .;

   *(.userdata*)		/* reserved for custom  */
  __allot_end__ = (. + 3) & (-4);  
				/* heap and stack space*/ 
  __initial_sp = 0x00020000;
  } > ram

  .flashdata :
  {
    KEEP (*(.flashdata*))  /* Flash Data */
    . = 0x0400;		/* length  limit */
  } > da_mem

  .debugormode :
  {
    KEEP (*(.modeconfig*))  /* Mode */
  } > mode_mem

  .protectmode :
  {
    KEEP (*(.protectconfig*))  /* Protect */
  } > pro_mem

  .eeprom :
  {
    KEEP (*(.eeprom*))  /* EEPROM */
    . = 0x1000;        /* length  limit */
  } > ee_mem

  .config1 :
  {
    KEEP (*(.config1*))  /* reserved config 1 */
  } > config_mem1

  .config2 :
  {
    KEEP (*(.config2*))  /* reserved config 2 */
  } > config_mem2



  /* Stabs debugging sections.  */
  .stab          0 : { *(.stab) }
  .stabstr       0 : { *(.stabstr) }
  .stab.excl     0 : { *(.stab.excl) }
  .stab.exclstr  0 : { *(.stab.exclstr) }
  .stab.index    0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment       0 : { *(.comment) }
  /* DWARF debug sections.
     Symbols in the DWARF debugging sections are relative to the beginning
     of the section so we begin them at 0.  */
  /* DWARF 1 */
  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }
  /* GNU DWARF 1 extensions */
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }
  /* DWARF 1.1 and DWARF 2 */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  /* DWARF 2 */
  .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line .debug_line.* .debug_line_end ) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }
  /* SGI/MIPS DWARF 2 extensions */
  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }
  /* DWARF 3 */
  .debug_pubtypes 0 : { *(.debug_pubtypes) }
  .debug_ranges   0 : { *(.debug_ranges) }
  /* DWARF Extension.  */
  .debug_macro    0 : { *(.debug_macro) }
  .gnu.attributes 0 : { KEEP (*(.gnu.attributes)) }
  /DISCARD/ : { *(.note.GNU-stack) *(.gnu_debuglink) *(.gnu.lto_*) }


}
