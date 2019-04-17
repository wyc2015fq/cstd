# Linux下SPI读写外部寄存器的操作 - DoubleLi - 博客园






SPI写寄存器操作：



**static****void** mcp251x_write_reg(**struct** spi_device *spi, uint8_t reg, uint8_t val)   
{   
**struct** mcp251x *chip = dev_get_drvdata(&spi->dev);   
**int** ret;   

    down(&chip->lock);   

    chip->spi_transfer_buf[0] = INSTRUCTION_WRITE;   
    chip->spi_transfer_buf[1] = reg;   
    chip->spi_transfer_buf[2] = val;   

    ret = spi_write(spi, chip->spi_transfer_buf, 3);   
**if** (ret < 0)   
        dev_dbg(&spi->dev, "%s: failed: ret = %d\n", __FUNCTION__, ret);   

    up(&chip->lock);   
}  





**static****void** mcp251x_write_bits(**struct** spi_device *spi, uint8_t reg, uint8_t mask, uint8_t val)  
{   
**struct** mcp251x *chip = dev_get_drvdata(&spi->dev);   
**int** ret;   

    down(&chip->lock);   

    chip->spi_transfer_buf[0] = INSTRUCTION_BIT_MODIFY;   
    chip->spi_transfer_buf[1] = reg;   
    chip->spi_transfer_buf[2] = mask;   
    chip->spi_transfer_buf[3] = val;   

    ret = spi_write(spi, chip->spi_transfer_buf, 4);   
**if** (ret < 0)   
        dev_dbg(&spi->dev, "%s: failed: ret = %d\n", __FUNCTION__, ret);   

    up(&chip->lock);   
}  



SPI读寄存器操作：



**static** uint8_t mcp251x_read_reg(**struct** spi_device *spi, uint8_t reg)   
{   
**struct** mcp251x *chip = dev_get_drvdata(&spi->dev);   
    uint8_t *tx_buf, *rx_buf;   
    uint8_t val;   
**int** ret;   

    tx_buf = chip->spi_transfer_buf;   
    rx_buf = chip->spi_transfer_buf + 8;   

    down(&chip->lock);   

    tx_buf[0] = INSTRUCTION_READ;   
    tx_buf[1] = reg;   
    ret = spi_write_then_read(spi, tx_buf, 2, rx_buf, 1);   
**if** (ret < 0)   
    {   
        dev_dbg(&spi->dev, "%s: failed: ret = %d\n", __FUNCTION__, ret);   
        val = 0;   
    }   
**else**
        val = rx_buf[0];   

    up(&chip->lock);   

**return** val;   
}  





**static** uint8_t mcp251x_read_state(**struct** spi_device *spi, uint8_t cmd)   
{   
**struct** mcp251x *chip = dev_get_drvdata(&spi->dev);   
    uint8_t *tx_buf, *rx_buf;   
    uint8_t val;   
**int** ret;   

    tx_buf = chip->spi_transfer_buf;   
    rx_buf = chip->spi_transfer_buf + 8;   

    down(&chip->lock);   

    tx_buf[0] = cmd;   
    ret = spi_write_then_read(spi, tx_buf, 1, rx_buf, 1);   
**if** (ret < 0)   
    {   
        dev_dbg(&spi->dev, "%s: failed: ret = %d\n", __FUNCTION__, ret);   
        val = 0;   
    }   
**else**
        val = rx_buf[0];   

    up(&chip->lock);   

**return** val;   
}  









