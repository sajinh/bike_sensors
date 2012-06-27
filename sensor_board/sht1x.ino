void sample_sht1x_data()
{
  delay(READ_SHT1X_WAIT_TIME);
  temp_c = sht1x.readTemperatureC();
  delay(READ_SHT1X_WAIT_TIME);
  humidity = sht1x.readHumidity();
}
