# Ready-Set-Boole- 🧮💡

Projet pédagogique autour de la logique booléenne, l'arithmétique des circuits logiques, et la transformation de formules logiques. Les exercices sont indépendants et illustrent différents aspects du calcul booléen, de l'évaluation de formules à la transformation en différentes formes normales.

## 📂 Structure du projet

Le dépôt est composé de plusieurs dossiers `ex00`, `ex01`, ..., `ex11`, chacun correspondant à un exercice ciblé :

- **ex00 à ex11/** : Chaque dossier contient un `Makefile` pour compiler l'exercice et des fichiers source (principalement `.cpp`).

## 🔧 Principaux exercices et fonctionnalités

### 1. Calcul booléen et arithmétique
- `ex01/multiplier.cpp` : Implémente un multiplicateur binaire en C++ utilisant des opérations sur bits.
- Utilisation typique :  
  ```bash
  ./multiplier <a> <b>
  ```
  Affiche le résultat de la multiplication de deux entiers.

### 2. Manipulation et évaluation de formules logiques

#### 📐 Structures principales
- Les fichiers tels que `ex05/negation_normal_form.cpp`, `ex06/conjunctive_normal_form.cpp`, `ex07/sat.cpp` définissent une structure `Expr` qui permet de représenter des formules logiques sous forme d'arbres :
  - Variables (`VAR` 🅰️)
  - Opérateurs logiques : NON (`NOT` 🚫), ET (`AND` ➕), OU (`OR` ➖), IMP (`IMP` ➡️), EQUIV (`IFF` 🔄), XOR (`XOR` ❎)
- Fonctions pour construire et manipuler ces arbres :  
  - `Var`, `Not`, `And`, `Or`, `Imp`, `Iff`, `Xor`
  - Transformations : passage en forme normale négative (`toNNF`), élimination des opérateurs complexes (`eliminateComplexOps`), distribution des OU sur les ET (`distributeOrOverAnd`).

#### 🔄 Exemples de transformations
- **Forme Normale Négative (NNF) :**  
  Convertit une formule logique afin que les négations ne portent que sur les variables.
- **Forme Normale Conjonctive (CNF) :**  
  Transforme une formule en conjonction de disjonctions, utile pour les solveurs SAT.
- **Évaluation d'ensembles** (`ex09/eval_set.cpp`):  
  Évalue des ensembles de valeurs selon une formule logique donnée.

### 3. ⚙️ Compilation
Chaque exercice dispose d’un `Makefile` permettant une compilation simple :
```bash
make
```
Le binaire est généré dans le dossier de l’exercice.

## 🚀 Utilisation générale

1. Se placer dans le dossier voulu (exemple `ex07`)
2. Compiler avec `make`
3. Exécuter le binaire généré (exemple `./sat`)

## 🖥️ Technologies utilisées

- Langage principal : **C++**
- Compilation : **Makefile** pour chaque exercice
- Structures de données : arbres, piles, vecteurs, maps

## 🎓 Exemples d'applications :

- **Transformation automatique de formules logiques** pour faciliter leur résolution.
- **Simulation de circuits logiques** et calcul d'expressions booléennes.
- **Outils pédagogiques** pour l’apprentissage de la logique et des circuits.

## 🗂️ Organisation des sources

- Chaque dossier contient :
  - Un ou plusieurs fichiers sources `.cpp`
  - Un `Makefile`
- La structure `Expr` permet de manipuler les formules booléennes dans la plupart des exercices avancés.

## 📖 Pour aller plus loin

Consultez les sources pour chaque exercice pour découvrir les implémentations spécifiques.  
Des exemples de transformation et d’évaluation sont inclus dans les `main()` de chaque fichier source.
