void sample_dht11_data()
{
  delay(TH_SNSR_WAIT_TIME);
  temp_c = dht.readTemperature();
  humidity = dht.readHumidity();
}
