# Projet Serpent — kit étudiant

Le sujet complet est dans **Projet_Serpent.pdf**. En bref :

1. Ouvrez un terminal **dans ce dossier**.
2. Compilez : `compiler.bat` (Windows), ou `mingw32-make` / `make`.
3. Lancez les tests : `tests` (ou `tests 1` pour la seule étape 1).
4. Lancez le jeu : `serpent`.

Vous écrivez le corps des fonctions marquées `A_FAIRE` dans `src/types.c`,
`src/carte.c`, `src/serpent.c`, `src/partie.c` et `src/scores.c`.
Ne modifiez ni les `.h`, ni `tests.c`.

Sous Linux ou macOS, installez d'abord raylib (`sudo apt install libraylib-dev`,
`brew install raylib`) ; les tests, eux, n'en ont pas besoin.

Le dossier `raylib/` contient raylib 5.5 (licence zlib, voir `raylib/LICENSE`),
compilée pour Windows (MinGW-w64).
