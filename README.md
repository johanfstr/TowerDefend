# 🏰 Tower Defend — Projet C 2026

Jeu de tower defense complet développé en **C** avec **SDL2**, réalisé dans le cadre de l'UE Programmation en C (CC2).

**Binôme :** Pierre-Antoine Becker & Johan Forestier

---

## 🎮 Présentation

Tower Defend est un jeu automatique dans lequel des unités assaillantes (la **Horde**) avancent le long d'un chemin généré aléatoirement, tandis que le **Roi** place automatiquement des tours de défense pour protéger sa tour. La partie s'arrête lorsque la tour du roi tombe à 0 point de vie.

---

## ✨ Fonctionnalités

- **Génération procédurale** de chemins aléatoires (réécriture de `initChemin()`)
- **Algorithme de placement automatique** des tours de défense adapté au chemin généré
- **Deux factions** qui jouent automatiquement :
  - 🛡️ **Le Roi** — tours terrestres (`tourTerrestre`) et tours aériennes (`tourAerienne`)
  - ⚔️ **La Horde** — archers, chevaliers, dragons et gargouilles
- **Gestion des vagues** : apparition aléatoire d'unités de la horde au début du chemin à chaque tour
- **Système de combat** complet : détection de portée, tri par points de vie, attaque unique par tour
- **Gestion des unités via listes chaînées** (`TListePlayer`)
- **Rendu graphique 2D** avec SDL2 (sprites, animations d'attaque)
- **Sauvegarde et restauration** de partie via fichier binaire (`.tdb`) et fichier séquentiel (`.tds`)

---

## 🗂️ Structure du projet

```c
.
├── main.c              # Boucle principale du jeu
├── towerdefend.h       # Types, constantes et prototypes
├── towerdefend.c       # Noyau du jeu (logique, combats, déplacements)
├── maSDL.c             # Interface graphique SDL2
├── sauvegarde.c        # Sauvegarde/restauration binaire et séquentielle
└── sdl2_local/         # Bibliothèque SDL2 locale
```

---

## ⚔️ Caractéristiques des unités

| Unité        | PV  | Dégâts | Portée | Vitesse atk | Déplacement | Cible       | Position |
|--------------|-----|--------|--------|-------------|-------------|-------------|----------|
| Tour Sol     | 500 | 120    | 5      | 1.5s        | 0           | sol         | sol      |
| Tour Air     | 500 | 100    | 3      | 1.0s        | 0           | air         | sol      |
| Tour du Roi  | 800 | 180    | 4      | 1.2s        | 0           | sol & air   | sol      |
| Chevalier    | 400 | 250    | 1      | 1.5s        | 2.0 m/s     | sol         | sol      |
| Archer       | 80  | 120    | 3      | 0.7s        | 1.0 m/s     | sol & air   | sol      |
| Dragon       | 200 | 70     | 2      | 1.1s        | 2.0 m/s     | sol & air   | air      |
| Gargouille   | 80  | 90     | 1      | 0.6s        | 3.0 m/s     | sol & air   | air      |

---

## 🔄 Déroulement d'un tour

1. **Phase combat** — chaque unité détecte les ennemis à portée (`quiEstAPortee`), attaque le plus faible
2. **Phase déplacement** — les unités de la horde avancent le long du chemin (une seule unité par case)
3. **Phase création** — apparition probabiliste d'une nouvelle tour (15–50 %) ou d'une unité horde (5–60 %)
4. **Phase affichage** — rendu console (`affichePlateauConsole`) et graphique SDL2

> Les unités de la horde ignorent les tours de défense et ciblent uniquement la tour du roi.

---

## 💾 Sauvegarde

| Touche | Action |
|--------|--------|
| `s` | Sauvegarder en binaire (`partiebin.tdb`) |
| `c` | Charger depuis le binaire (`partiebin.tdb`) |
| `d` | Sauvegarder en séquentiel (`partieseq.tds`) |
| `v` | Charger depuis le séquentiel (`partieseq.tds`) |

---

## 🔧 Compilation

> Selon votre environnement, choisissez la commande adaptée :

**SDL2 dans le dossier courant**
```bash
gcc -std=c99 -g main.c maSDL.c towerdefend.c sauvegarde.c \
  -I sdl2_local/include/SDL2 \
  -L sdl2_local/lib \
  -lSDL2 -o projet
```

**(SDL2 installée système)**
```bash
gcc -std=c99 -Wall -g \
  main.c maSDL.c towerdefend.c sauvegarde.c \
  $(sdl2-config --cflags --libs) \
  -o projet
```

---

## ▶️ Lancement

```bash
./projet
```

---

## 📐 Plateau de jeu

Le plateau fait **11 colonnes × 19 lignes** (1 case = 1 mètre).
- La **tour du roi** est placée en haut du chemin dès le début de la partie
- Les **unités de la horde** apparaissent en bas du chemin et le remontent

---

*Projet réalisé dans le cadre de l'UE Programmation en C — 2026.*
