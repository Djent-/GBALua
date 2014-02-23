#ifndef GBAIRQ_H
#define GBAIRQ_H

//typedef void    Fv(void);
//typedef Fv      *Interrupt;

void VBlankIrq(void);
void HBlankIrq(void);
void VCountIrq(void);
void Timer0Irq(void);
void Timer1Irq(void);
void Timer2Irq(void);
void Timer3Irq(void);
void SIOIrq(void);
void DMA0Irq(void);
void DMA1Irq(void);
void DMA2Irq(void);
void DMA3Irq(void);
void KeyIrq(void);
void CartIrq(void);

void IRQHandler(void);
void IRQSetup(void);
  
#define VBLANK_IRQ_FLAG     0x0001      // V Blank Interrupt
#define HBLANK_IRQ_FLAG     0x0002      // H Blank Interrupt
#define VCOUNT_IRQ_FLAG     0x0004      // V Counter Corresponding Interrupt
#define TIMER0_IRQ_FLAG     0x0008      // Timer 0 Interrupt
#define TIMER1_IRQ_FLAG     0x0010      // Timer 1 Interrupt
#define TIMER2_IRQ_FLAG     0x0020      // Timer 2 Interrupt
#define TIMER3_IRQ_FLAG     0x0040      // Timer 3 Interrupt
#define SIO_IRQ_FLAG        0x0080      // Serial Communication Interrupt
#define DMA0_IRQ_FLAG       0x0100      // DMA0 Interrupt
#define DMA1_IRQ_FLAG       0x0200      // DMA1 Interrupt
#define DMA2_IRQ_FLAG       0x0400      // DMA2 Interrupt
#define DMA3_IRQ_FLAG       0x0800      // DMA3 Interrupt
#define KEY_IRQ_FLAG        0x1000      // Key Interrupt
#define CART_IRQ_FLAG       0x2000      // Game Pak Interrupt


#define REG_INTERUPT   *(unsigned*)0x3007FFC   //Interrupt Register

#endif

