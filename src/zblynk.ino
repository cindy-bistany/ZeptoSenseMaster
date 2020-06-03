// emacs -*- c++ -*-
//zblynk - glue code between blynk and zeptive

void blynk_status_message(String msg)
{
  Blynk.virtualWrite(V30, msg);
}
