void geiger_takes_first_sample()
{
  while ( (millis() - lastMills) <= sampling_time){}
   geig_count = get_geiger_count() ;
   lastMills += (sampling_time) ;
}

void geiger_updates_sample()
{
  while ( (millis() - lastMills) <= update_time){}
   unsigned long curr_count =  get_geiger_count() ;
   curr_count *= tratio;
   float tmp_count = geig_count*weight + (1-weight)*curr_count;
   geig_count = long(tmp_count);
   lastMills += update_time ; 
}
