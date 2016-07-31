#include "lpc17xx_uart.h"

//====================================================================================
void retarget_init()
{
  // Initialize UART
}

//====================================================================================
int _write (int fd, char *ptr, int len)
{
	return(UART_Send(LPC_UART0, ptr, len, BLOCKING));
}

//====================================================================================
int _read (int fd, char *ptr, int len)
{
  /* Read "len" of char to "ptr" from file id "fd"
   * Return number of char read.
   * Need implementing with UART here. */
  return len;
}

//====================================================================================
void _ttywrch(int ch) {
  /* Write one char "ch" to the default console
   * Need implementing with UART here. */
}


