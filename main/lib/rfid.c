// https://github.com/asif-mahmud/MIFARE-RFID-with-AVR/blob/master/lib/avr-rfid-library/lib/mfrc522.c
// RFID component used: MFRC522
#include <rfid.h>
#include <spi.h>

void rfid_init() {
  uint8_t byte;
  rfid_reset();
  
  rfid_write(TModeReg, 0x8D);
  rfid_write(TPrescalerReg, 0x3E);
  rfid_write(TReloadReg_1, 30);   
  rfid_write(TReloadReg_2, 0); 
  rfid_write(TxASKReg, 0x40);  
  rfid_write(ModeReg, 0x3D);
  
  byte = rfid_read(TxControlReg);
  if(!(byte&0x03))
  {
    rfid_write(TxControlReg,byte|0x03);
  }
}

void rfid_write(uint8_t reg, uint8_t data) {
  ENABLE_CHIP();
  spi_transmit((reg<<1)&0x7E);
  spi_transmit(data);
  DISABLE_CHIP();
}

uint8_t rfid_read(uint8_t reg) {
  uint8_t data; 
  ENABLE_CHIP();
  spi_transmit(((reg<<1)&0x7E)|0x80);
  data = spi_transmit(0x00);
  DISABLE_CHIP();
  return data;
}

void rfid_reset() {
  rfid_write(CommandReg,SoftReset_CMD);
}

uint8_t rfid_request(uint8_t req_mode, uint8_t * tag_type) {
  uint8_t  status;  
  uint32_t backBits;  //The received data bits

  rfid_write(BitFramingReg, 0x07);  //TxLastBists = BitFramingReg[2..0]  ???
  
  tag_type[0] = req_mode;
  status = rfid_to_card(Transceive_CMD, tag_type, 1, tag_type, &backBits);

  if ((status != CARD_FOUND) || (backBits != 0x10)) {    
    status = ERROR;
  }
   
  return status;
}

uint8_t rfid_to_card(uint8_t cmd, uint8_t *send_data, uint8_t send_data_len, 
                     uint8_t *back_data, uint32_t *back_data_len) {
  uint8_t status = ERROR;
  uint8_t irqEn = 0x00;
  uint8_t waitIRq = 0x00;
  uint8_t lastBits;
  uint8_t n;
  uint8_t tmp;
  uint32_t i;

  switch (cmd) {
    case MFAuthent_CMD:   //Certification cards close
      irqEn = 0x12;
      waitIRq = 0x10;
      break;

    case Transceive_CMD:    //Transmit FIFO data
      irqEn = 0x77;
      waitIRq = 0x30;
      break;

    default:
      break;
  }
 
  n=rfid_read(ComIrqReg);
  rfid_write(ComIrqReg,n&(~0x80));  //clear all interrupt bits
  n=rfid_read(FIFOLevelReg);
  rfid_write(FIFOLevelReg,n|0x80);  //flush FIFO data
    
  rfid_write(CommandReg, Idle_CMD); //NO action; Cancel the current cmd???

  //Writing data to the FIFO
  for (i=0; i<send_data_len; i++) {   
    rfid_write(FIFODataReg, send_data[i]);    
  }

  //Execute the cmd
  rfid_write(CommandReg, cmd);
  if (cmd == Transceive_CMD) {    
    n=rfid_read(BitFramingReg);
    rfid_write(BitFramingReg,n|0x80);  
  }   
    
  //Waiting to receive data to complete
  i = 2000; //i according to the clock frequency adjustment, the operator M1 card maximum waiting time 25ms???
  do {
    //CommIrqReg[7..0]
    //Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
    n = rfid_read(ComIrqReg);
    i--;
  } while ((i!=0) && !(n&0x01) && !(n&waitIRq));

  tmp=rfid_read(BitFramingReg);
  rfid_write(BitFramingReg,tmp&(~0x80));
  
  if (i != 0) {    
    if (!(rfid_read(ErrorReg) & 0x1B)) { //BufferOvfl Collerr CRCErr ProtecolErr
      status = CARD_FOUND;
      
      if (n & irqEn & 0x01) {   
        status = CARD_NOT_FOUND;      //??   
      }

      if (cmd == Transceive_CMD) {
        n = rfid_read(FIFOLevelReg);
        lastBits = rfid_read(ControlReg) & 0x07;

        if (lastBits) {   
          *back_data_len = (n-1)*8 + lastBits;   
        } else {   
          *back_data_len = n*8;   
        }

        if (n == 0)  
          n = 1;    
              
        if (n > MAX_LEN) 
          n = MAX_LEN;   
      
        //Reading the received data in FIFO
        for (i=0; i<n; i++) {   
          back_data[i] = rfid_read(FIFODataReg);    
        }
      }

    } else {   
      status = ERROR;  
    } 
  }

    return status;
}


uint8_t rfid_get_card_serial(uint8_t * serial_out) {
  uint8_t status;
  uint8_t i;
  uint8_t serNumCheck=0;
  uint32_t unLen;
    
  rfid_write(BitFramingReg, 0x00);   //TxLastBists = BitFramingReg[2..0]
 
  serial_out[0] = PICC_ANTICOLL;
  serial_out[1] = 0x20;
  status = rfid_to_card(Transceive_CMD, serial_out, 2, serial_out, &unLen);

  if (status == CARD_FOUND) {
    //Check card serial number
    for (i=0; i<4; i++) {   
      serNumCheck ^= serial_out[i];
    }

    if (serNumCheck != serial_out[i]) {   
      status = ERROR;    
    }
  }
  
  return status;
}
