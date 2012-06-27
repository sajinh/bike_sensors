void wire_sensor_data(EthernetClient client)
{
  if (client) {
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/plain");
          client.println();
          long temp=temp_c*100;
          long humi=humidity*100;
          long radi=sievert;
          long pres=(slp-800)*10;
            client.print("T=");
            client.print(temp);
            client.print("&H=");
            client.print(humi);
            client.print("&P=");
            client.print(pres);
            client.print("");
            client.print("&R=");
            client.print(radi);
            client.println("");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
  }
  client.stop();
}
