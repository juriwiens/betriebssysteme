/**
Betriebssysteme Blatt 18, Aufgabe 18-2

Datei: 18-2.c
Autor: Juri Wiens (Matrikelnr: 9023765)
Übungsgruppe: 5
Erstellt: 07.06.2015

Aufgabe:
Wie hoch ist die Auslastung bei einem Serverprozeß mit durchschnittlich 3.2
Anfragen in der Warteschlange?

Vorüberlegungen:
  Notationshinweise:
  x**y entspricht x potenziert mit y.
  sqrt() entspricht der Quadratwurzel.

  Lq = 3.2 = ρ**2 / (1 - ρ)
  => ρ**2 = 3.2 - 3.2ρ
  => ρ**2 + 3.2ρ - 3.2 = 0
  => ρ = -3.2/2 +- sqrt((3.2/2)**2 - (-3.2))
  => ρ = -8/5 +- sqrt(64/25 + 80/25)
  => ρ = -8/5 +- sqrt(144/25)
  => ρ = -8/5 +- 12/5
  => ρ = 4/5 v ρ = -20/5

  Allgemein: ρ = -(Lq/2) +- sqrt((Lq/2)**2 + Lq)

  Interpretation: Da per Definition ρ = λ / μ, folgt aus ρ = -20/5, oder auch
  allgemein aus einem negativen Wert, dass entweder λ oder μ einen negativen
  Wert haben müssten. Eine negative Anzahl ankommender oder bedienbarer
  Einheiten erscheint mir jedoch in dem gegebenen Kontext nicht sinnvoll, daher
  ignoriere ich diesen Wert und nehme 4/5 als einziges korrektes Ergebnis an.

Aufgabe:
Wie hoch ist die durchschnittliche Wartezeit Wq im tcp-backlog bei einer
Ankunftsrate λ = 8/s?

Vorüberlegungen:
  Was ist ein tcp-backlog?
  Die maximale Größe der Warteschlange für eingehende TCP Verbindungen.

  Lq = λ * Wq
  => Wq = Lq / λ

  Mit λ = 8/s gilt also:
  Wq = Lq/8 s

  Annahme: Lq = 3.2 (aus vorheriger Aufgabe)
  => Wq = 3.2/8 s = 0.4 s

Ergebnis:
Die durchschnittliche Wartezeit beträgt 0.4 Sekunden.

Nutzungshinweise:
Aufruf ohne Argumente.

Verwendete Systemfunktionen:
  double sqrt(double x) :
  Gibt die Quadratwurzel von x zurück.
*/

#include <stdio.h> // printf
#include <math.h> // sqrt

/** Calculates the utilization ρ, given the average queue length Lq */
double calc_utilization(double Lq) {
  double res1, res2;

  res1 = -(Lq/2) + sqrt(pow(Lq/2,2) + Lq);
  res2 = -(Lq/2) - sqrt(pow(Lq/2,2) + Lq);

  return res1 > 0 ? res1 : res2;
}

/**
 * Calculates the average waiting time Wq in seconds, given the average queue
 * length Lq and the arrival rate λ (with time unit: seconds).
 */
double calc_average_waitingtime(double Lq, double l) {
  return Lq/l;
}

int main(void) {
  double r;
  double Wq;
  double Lq = 3.2;
  double l = 8;

  r = calc_utilization(Lq);
  printf("ρ = -(%f/2) +- sqrt((%f/2)**2 + %f) = %f\n", Lq, Lq, Lq, r);

  Wq = calc_average_waitingtime(Lq, l);
  printf("Wq = %f/%f s = %f s\n", Lq, l, Wq);

  return 0;
}
