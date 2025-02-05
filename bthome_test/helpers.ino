void timeToByteArray(time_t timeValue, uint8_t* byteArray) {
    // Convert the time_t value to a byte array in little-endian order
    byteArray[0] = 0;
    byteArray[1] = 0;
    byteArray[2] = 0;
    byteArray[3] = 0;
    byteArray[4] = 0;
    byteArray[5] = 0;
}