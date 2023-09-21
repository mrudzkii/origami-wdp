#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef NAN
#endif

bool rowne_zero(double x) { return fabs(x) < 1e-6; }

typedef struct punkt {
  double x, y;
} punkt;

// Struktura kartka posiada wszystkie zmienne, które potencjalnie mogą się przydać,
// niezależnie od sposobu, w jaki dana kartka powstała. Zmienna nieużywana przez
// dany typ kartki będzie miała wartość -1 (dla inta) lub NAN (dla double'a)
typedef struct kartka {
  char postac;
  int zginana_kartka;
  struct punkt p1, p2;
  double promien;
} kartka;

typedef struct pytanie {
  int kartka;
  struct punkt p;
} pytanie;

punkt odbicie_wzgledem_prostej(punkt punkt_odbijany, punkt p1, punkt p2) {
  // Wzory wyprowadziłem korzystając z równania ogólnego:
  // Prostej danej: ax + by + c = 0
  // oraz faktu, że jeżeli dwie proste zadane równaniem ogólnym są prostopadłe,
  // to ich współczynniki spełniają warunek a*a' + b*b' = 0, więc prosta
  // prostopadła do danej, przechodząca przez punkt dany (ale również przez
  // jego odbicie względem tej prostej) może mieć postć: bx - ay + d = 0
  // Wtedy współrzedne punktu przecięcia tych dwóch prostych są średnią
  // arytmetyczną współrzędnych punktów danego i szukanego.
  double a = p2.y - p1.y;
  double b = p1.x - p2.x;
  double c = -p1.y * p1.x + p1.y * p2.x + p1.y * p1.x - p1.x * p2.y;
  punkt wyn;
  if (!rowne_zero(a)) {  //case dla prostej poziomej
    double d = a * punkt_odbijany.y - b * punkt_odbijany.x;
    double xsr = -(b * d + a * c) / (a * a + b * b);
    double ysr = xsr * b / a + d / a;
    wyn.x = 2 * xsr - punkt_odbijany.x;
    wyn.y = 2 * ysr - punkt_odbijany.y;
  } else {
    wyn.x = punkt_odbijany.x;
    wyn.y = -2 * c / b - punkt_odbijany.y;
  }
  return wyn;
}

int po_ktorej_stronie_prostej(punkt dany_punkt, struct punkt p1,
                              struct punkt p2) {
  double a = p1.y - p2.y;
  double b = p2.x - p1.x;
  double c = p1.x * p1.y + p1.x * p2.y - p1.x * p1.y - p2.x * p1.y;
  if (rowne_zero(a * dany_punkt.x + b * dany_punkt.y + c))
    return 0;
  else if (a * dany_punkt.x + b * dany_punkt.y + c < 0)
    return -1;
  else
    return 1;
}

int ile_warstw(punkt dany_punkt, kartka *kartki, int n) {
  if (kartki[n].postac == 'K') {
    if ((dany_punkt.x - kartki[n].p1.x) * (dany_punkt.x - kartki[n].p1.x) +
            (dany_punkt.y - kartki[n].p1.y) * (dany_punkt.y - kartki[n].p1.y) <=
        kartki[n].promien * kartki[n].promien)
      return 1;
    else {
      return 0;
    }
  } else if (kartki[n].postac == 'P') {
    if (dany_punkt.x >= kartki[n].p1.x && dany_punkt.x <= kartki[n].p2.x &&
        dany_punkt.y >= kartki[n].p1.y && dany_punkt.y <= kartki[n].p2.y)
      return 1;
    else {
      return 0;
    }
  } else if (kartki[n].postac == 'Z' &&
             po_ktorej_stronie_prostej(dany_punkt, kartki[n].p1,
                                       kartki[n].p2) == -1) {
    return 0;
  } else if (kartki[n].postac == 'Z' &&
             po_ktorej_stronie_prostej(dany_punkt, kartki[n].p1,
                                       kartki[n].p2) == 0) {
    return ile_warstw(dany_punkt, kartki, kartki[n].zginana_kartka - 1);
  } else {
    return ile_warstw(dany_punkt, kartki, kartki[n].zginana_kartka - 1) +
           ile_warstw(
               odbicie_wzgledem_prostej(dany_punkt, kartki[n].p1, kartki[n].p2),
               kartki, kartki[n].zginana_kartka - 1);
  }
}

int main() {
  int lkartek, lpytan;
  int tmp;
  tmp = scanf("%d %d\n", &lkartek, &lpytan);
  kartka *kartki = (kartka *)malloc(sizeof(kartka) * (size_t)lkartek);

  for (int i = 0; i < lkartek; i++) {
    tmp = scanf("%c", &kartki[i].postac);
    if (kartki[i].postac == 'P') {
      kartki[i].zginana_kartka = -1;
      tmp = scanf("%lf %lf %lf %lf\n", &kartki[i].p1.x, &kartki[i].p1.y,
                    &kartki[i].p2.x, &kartki[i].p2.y);
      kartki[i].promien = NAN;
    } else if (kartki[i].postac == 'K') {
      kartki[i].zginana_kartka = -1;
      kartki[i].p2.x = NAN;
      kartki[i].p2.y = NAN;
      tmp = scanf("%lf %lf %lf\n", &kartki[i].p1.x, &kartki[i].p1.y,
                    &kartki[i].promien);
    } else {
      tmp = scanf("%d %lf %lf %lf %lf\n", &kartki[i].zginana_kartka,
                    &kartki[i].p1.x, &kartki[i].p1.y, &kartki[i].p2.x,
                    &kartki[i].p2.y);
      kartki[i].promien = NAN;
    }
  }
  pytanie *pytania = (pytanie *)malloc(sizeof(pytanie) * (size_t)lpytan);
  for (int i = 0; i < lpytan; i++) {
    tmp = scanf("%d %lf %lf", &pytania[i].kartka, &pytania[i].p.x,
                  &pytania[i].p.y);
  }

  for (int i = 0; i < lpytan; i++) {
    printf("%d\n", ile_warstw(pytania[i].p, kartki, pytania[i].kartka - 1));
  }
  tmp = tmp;
  free(kartki);
  free(pytania);
  return 0;
}
