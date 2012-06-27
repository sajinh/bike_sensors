void sample_pressure_sensor(float elevation)
{
/* get the pressure */
  val = analogRead(pressurePin);
  pkPa = ((float)val/(float)1023+0.095)/0.009;
  
  slp = (-1.0*elevation)/((temp_c+273.15)*29.263);
  slp = pkPa*10/exp(slp);
  
}
