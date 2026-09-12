# c-battleships

Schiffeversenken als Terminal-Spiel in C, entstanden als Projekt im Modul "Prozedurale Programmierung". Gespielt wird gegen einen Computergegner auf zwei 10x10-Spielfeldern.

## Features

- Klassisches Schiffeversenken: eigenes Feld + Gegnerfeld werden im Terminal gerendert
- Spiel gegen einen COM-Gegner mit eigener Schusslogik
- Eingabe von Koordinaten im Format `Buchstabe Zahl` (z. B. `A 3`), Spalten A–J, Zeilen 0–9
- Erkennung von Treffer, Fehlschuss und versenktem Schiff
- Weiterschießen bei Treffer, Zugwechsel bei Fehlschuss

## Tech Stack

- C (Standard C99)
- Kompiliert über ein Makefile mit `gcc`

## Projektstruktur

```
cpp-battleships/
├── main.c         # Game-Loop und States (Setup, Spielerzug, COM-Zug)
├── board.c/.h      # Spielbrett-Logik (Platzieren, Schießen, Status)
├── player.c/.h     # Spieler-Eingaben und Schiffsplatzierung
├── enemy.c/.h      # COM-Gegner (Zufallszüge)
├── render.c/.h     # Terminal-Ausgabe der Spielfelder
├── Makefile
└── PPI Projekt - Schiffe versenken.pdf   # Projektbeschreibung
```

## Installation & Build

Voraussetzung: `gcc` und `make`.

```bash
git clone https://github.com/NuriYildirim/cpp-battleships.git
cd cpp-battleships
make
```

## Verwendung

```bash
./game
```

Zu Beginn platzieren Spieler und COM ihre Schiffe. Danach wechseln sich Spieler und COM mit Schüssen ab — Koordinate im Format `A 3` eingeben und mit Enter bestätigen. Bei einem Treffer darf man direkt weiterschießen, bei einem Fehlschuss ist die Gegenseite an der Reihe.
