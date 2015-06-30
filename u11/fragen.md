# Fragen und Antworten zu Blatt 11

*Juri Wiens, Übungsgruppe 5*

#### 1. Welche Anwendungsmöglichkeiten bieten Message Queues?

Message Queues können zum Beispiel in der Software-Architektur als Task/Job Queues eingesetzt werden. So können zu erledigende Aufgaben von einem Programm in eine Queue geschrieben werden, welches damit die Verantwortung für diese abgibt. Diese Aufgaben können dann von einer beliebigen Anzahl von Workern übernommen werden. Worker können dabei fremde Prozesse, aber auch Threads innerhalb des selben Programms sein. Eine solche Architektur kann richtig angewendet und im Vergleich zu einer monolithischen also zu einer loseren Kopplung von einzelnen, spezialisierten Programmen bzw. Programmmodulen führen, die sich besser skalieren lassen.

#### 2. Was haben Message Queues und Shared Memory gemeinsam?

Sowohl Message Queues als auch Shared Memory ermöglichen den Austausch von Daten zwischen Prozessen. Beide halten Daten "kernel-persistent", speichern die Daten also bis zu einem Neustart, falls diese nicht vorher explizit gelöscht werden. Auch sind deren APIs sehr ähnlich: um zum Beispiel eine Queue bzw. ein Shared-Memory-Segment zu erstellen oder referenzieren ist ein Key notwendig, zu welchem man über eine get Funktion eine entsprechende ID erhält.

#### 3. Was unterscheidet Message Queues von Named Pipes einerseits und Shared Memory andererseits?

Der grundlegende Unterschied zwischen Message Queues und Named Pipes besteht darin, dass in Named Pipes beliebige Daten geschrieben werden können, wohingegen Message Queues nachrichtenorientiert sind. Damit agieren letztere auf einer höheren Abstraktionsebene, mit allen damit verbundenen Vor- und Nachteilen wie geringerer Kontrolle, eventuellem Verlust von Flexibilität und Performance, aber dafür geringerem Implementierungsbedarf. So sind Pipes in ihrer Größe prinzipiell unlimitiert, Message Queues haben vom System festgelegte Grenzen. Es resultiert daraus auch, dass Pipes über die Standardfunktionen des Dateisystems angesprochen werden können, wohingegen Message Queues über spezialisierte Funktionen angesprochen werden müssen. Weiterhin sind Pipes für eine unidirektionale "one-to-one" Kommunikation von genau einem Prozess zu genau einem anderem Prozess (welcher theoretisch aber auch der selbe sein kann) konzipiert. Bei Message Queues hingegen ist auch die "many-to-many" Kommunikation vorgesehen, da beliebige Prozesse (mit entsprechedem Zugriffrecht) Nachrichten an die Queue senden und aus dieser empfangen können.

Auch beim Vergleich von Message Queues und Shared Memory lässt sich eine unterschiedliche Abstraktionsebene feststellen, so dass die gleich prinzipiellen Vor- und Nachteile gelten. So lässt die Verwendung von Shared Memory dem Entwickler die volle Gestaltungsfreiheit über die Implementierung der Kommunikation, während die Verwendung von Queues diese in Form von Nachrichtenaustausch vorgibt. Im Gegenzug ist die Verwendung von Message Queues mit gewissen Garantien verbunden, z.B. dass das Senden und Empfangen von Nachrichten thread-safe ist und Nachrichten nur komplett geschrieben und gelesen werden können. Diese Garantien bestehen bei der Verwendung von Shared Memory nicht und müssten selber implementiert werden.
