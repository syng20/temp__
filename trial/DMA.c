
#include "DMA.h"
#include "SysTimer.h"

void DMA_Init(void) {
	// Enable Clock for DMA 1
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	// delay 20 microseconds
	delay(1);
	
	// Disable DMA 1 channel 6
	DMA1_Channel6->CCR &= ~DMA_CCR_EN;
	// Enable mem to mem
	DMA1_Channel6->CCR |= DMA_CCR_MEM2MEM;
	// Set channel priority to high (10)
	DMA1_Channel6->CCR &= ~DMA_CCR_PL;
	DMA1_Channel6->CCR |= DMA_CCR_PL_1;
	// set peripheral size = 32 bit (10)
	DMA1_Channel6->CCR &= ~DMA_CCR_PSIZE;
	DMA1_Channel6->CCR |= DMA_CCR_PSIZE_1;
	// memory size 32 (10)
	DMA1_Channel6->CCR &= ~DMA_CCR_MSIZE;
	DMA1_Channel6->CCR |= DMA_CCR_MSIZE_1;
	// disable peripheral increment mode
	DMA1_Channel6->CCR &= ~DMA_CCR_PINC;
	// Enable memory increment mode
	DMA1_Channel6->CCR |= DMA_CCR_MINC;
	// Disable circular mode
	DMA1_Channel6->CCR &= ~DMA_CCR_CIRC;
	// data transfer: memory to peripheral
	DMA1_Channel6->CCR |= DMA_CCR_DIR; // read from mem
	
	// data source to data buffer
	DMA1_Channel6->CMAR = (uint32_t) DataBuffer;
	// data destination to data register of CRC block
	DMA1_Channel6->CPAR = (uint32_t) &(CRC->DR);
	
	// disable half transfer interrupt
	DMA1_Channel6->CCR &= ~DMA_CCR_HTIE;
	// disable transfer error interrupt
	DMA1_Channel6->CCR &= ~DMA_CCR_TEIE;
	// Enable transfer complete interrupt
	DMA1_Channel6->CCR |= DMA_CCR_TCIE;
	
	// Enable NVIC
	NVIC_EnableIRQ(DMA1_Channel6_IRQn);
  NVIC_SetPriority(DMA1_Channel6_IRQn, 0);
}

void DMA1_Channel6_IRQHandler(void){ 
	// clear NVIC Interrupt Flag
	NVIC_ClearPendingIRQ(DMA1_Channel6_IRQn);
	// check transfer complete flag
	if ((DMA1->ISR & DMA_ISR_TCIF6) == DMA_ISR_TCIF6) {
		DMA1->IFCR |= DMA_IFCR_CTCIF6;
		completeCRC(CRC->DR);
	}
	// clear global DMA interrupt
	if ((DMA1->ISR & DMA_ISR_GIF6) == DMA_ISR_GIF6) 
		DMA1->IFCR |= DMA_IFCR_CGIF6;
}
