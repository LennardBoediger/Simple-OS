* Register sichern
* Grund für Ausnahme
* Grund behandeln
* Rücksprungadresse modifizieren (IRQ/FIQ -4 (nahtlos? letzte abgeschlossene?;
                                  Data_Abort -8; Undef -0; Pref_Abort -4)
                                  # ACHTUNG: Rechnung wahrscheinlich falsch. Alle +4 #
                                  ## ACHTUNGACHTUNG: wahrscheinlich doch richtig ##
* Register wiederherstellen
*Exeptionmode ausstellen
--> Fortsetzen der eigentlichen Ausführung

Bei RESET: Nur an Anfang des Programms springen
?? Bei SWI: NIX machen?

PRIORITÄT: Reset, Data_abort, FIQ, IRQ, Pref_abort, undef/SWI
?? Wie stellt man Prioritätskategorie fest?
Bei undef hatten wir 10011011, sollte es nicht 0 am Anfang sein?