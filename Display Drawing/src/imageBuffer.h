

uint8_t *imgBuffer = NULL;
void setupImageBuffer();
void clearImageBuffer();
void sendImageToDisplay(); // upload image buffer to display buffer

//-------------------------------------------------
void setupImageBuffer() {
  Serial.println("Setup image buffer");
#ifdef ESP32 // ESP32-S3 with PSRAM
  imgBuffer = (uint8_t *)ps_malloc((WIDTH * HEIGHT) / 8);
#else
  imgBuffer = (uint8_t *)malloc((WIDTH * HEIGHT) / 8);
#endif
  clearImageBuffer();
}

//-------------------------------------------------
void clearImageBuffer() {
  // all pixels to white
  memset(imgBuffer, 0xff, (width * height) / 8);
}

//-------------------------------------------------
void Game::sendImageToDisplay() {
  // Copies image buffer to displayBuffer of Sharp memory display
  // just set our image to the display as eg. background
  display.setBitmap(imgBuffer);
}