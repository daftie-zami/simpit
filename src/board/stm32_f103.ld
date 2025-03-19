/*
 * This is a generic linker script for Cortex-M targets using libopencm3.
 *
 * Memory regions MUST be defined in the ld script which includes this one!
 * Example:
*/

MEMORY
{
	rom (rx) : ORIGIN = 0x08000000, LENGTH = 256K
	ram (rwx) : ORIGIN = 0x20000000, LENGTH = 16K
}

INCLUDE cortex-m-generic.ld
