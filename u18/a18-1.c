/**
Betriebssysteme Blatt 18, Aufgabe 18-1

Datei: a18-1.c
Autor: Juri Wiens (Matrikelnr: 9023765)
Übungsgruppe: 5
Erstellt: 06.06.2015

Aufgabe:
Die Ankünfte von Anfragen an ein Serversystem mögen einem homogenen Poisson-
Prozess genügen. Wie groß ist die Wahrscheinlichkeit P, dass in einer
Zeitspanne von 4 Sekunden bei einer Auslastung von ρ = 0.85 und einer Bedienrate
μ = 0.80 nicht mehr als 5 Anfragen eintreffen?

Vorüberlegungen:
Die Wahrscheinlichkeit von nicht mehr als 5 Anfragen entspricht der Summe
der Wahrscheinlichkeiten von 0 bis 5 Anfragen.

Ergebnis:
Die Wahrscheinlichkeit beträgt 0.941647

Nutzungshinweise:
Aufruf ohne Argumente.

Verwendete Systemfunktionen:
  double pow(double x, double y) :
  Gibt x hoch y zurück.

  double exp(double x) :
  Exponentialfunktion, die e hoch x zurückgibt.
*/

#include <stdio.h> // printf
#include <math.h> // pow, exp

/** Time span τ */
unsigned int t = 4;

/** Utilization ρ */
double r = 0.85;

/** Service rate μ */
double m = 0.80;

/** Arrival rate λ */
double l;

/** Request maximum */
unsigned int req_max = 5;

/**
 * Calculates and returns the arrival rate λ by using:
 * ρ = λ / μ => λ = ρ * μ
 */
double calc_arrival_rate(double r, double m) {
  return r * m;
}

/** Returns the factorial of n */
unsigned int factorial(unsigned int n) {
  unsigned int ret = 1;

  for (int i = n; i > 1; --i) {
    ret *= i;
  }

  return ret;
}

/**
 * Returns the propability P for i arrivals during a time span t and in
 * consideration of an arrival rate l by using a poisson process.
 */
double poisson(double l, double t, unsigned int i) {
  double lt = l * t;

  double factor = pow(lt, i) / factorial(i);
  double e_val = exp(-lt);

  return factor * e_val;
}

int main(void) {
  double propability = 0.0;
  double accumulated_propability = 0.0;

  l = calc_arrival_rate(r, m);
  printf("λ = ρ * μ = %f * %f = %f\n", r, m, l);

  for (int i = 0; i <= req_max; i++) {
    propability = poisson(l, t, i);
    printf("P(λ = %f, τ = %u, i = %u) = %f\n", l, t, i, propability);
    accumulated_propability += propability;
  }

  printf("Accumulated propability for 0 to %u requests: %f\n", req_max,
      accumulated_propability);

  return 0;
}
