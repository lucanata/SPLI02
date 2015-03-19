# SPLI02

Modifiche debugging effettuate (già presenti):
- aggiunto stuttura dati u_char *dati (mancava in analyzer.c)
- modificato Makefile aggiungendo -lpcap alla compilazione invece della posizione della libreria /lib/....etc

Per farlo andare:
entrare nella cartella pcap ed eseguire:
./configure
sudo make install

dentro a SPLI02/Modified_codes:
make
modificare l'ip nel file genaral.conf con il proprio e cambiare en0 con la propria interfaccia

per lanciare lo sniffer:
./analyzer general.conf 


!!!--- NON FATE IL COMMIT DI GENERAL.CONF ALTRIMENTI OGNUNO SE LO DEVE CAMBIARE SEMPRE CON LA SUA INTERFACCIA ---!!!
