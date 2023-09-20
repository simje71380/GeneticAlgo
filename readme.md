<div align="center">
<h1 align="center">
<img src="https://raw.githubusercontent.com/PKief/vscode-material-icon-theme/ec559a9f6bfd399b82bb44393651661b08aaf7ba/icons/folder-markdown-open.svg" width="100" />
<br>Genetic Algorithm Implementation for Employee Routing and Scheduling Optimization in a SESSAD Context
</h1>
<h3>◦ Developed with the software and tools listed below.</h3>

<p align="center">
<img src="https://img.shields.io/badge/C-A8B9CC.svg?style&logo=C&logoColor=black" alt="C" />
<img src="https://img.shields.io/badge/Markdown-000000.svg?style&logo=Markdown&logoColor=white" alt="Markdown" />
</p>
<img src="https://img.shields.io/github/languages/top/simje71380/GeneticAlgo?style&color=5D6D7E" alt="GitHub top language" />
<img src="https://img.shields.io/github/languages/code-size/simje71380/GeneticAlgo?style&color=5D6D7E" alt="GitHub code size in bytes" />
<img src="https://img.shields.io/github/commit-activity/m/simje71380/GeneticAlgo?style&color=5D6D7E" alt="GitHub commit activity" />
<img src="https://img.shields.io/github/license/simje71380/GeneticAlgo?style&color=5D6D7E" alt="GitHub license" />
</div>

---

## 📒 Table of Contents
- [📒 Table of Contents](#-table-of-contents)
- [📍 Overview](#-overview)
- [🚀 Getting Started](#-getting-started)
  - [✔️ Prerequisites](#️-prerequisites)
  - [💻 Installation](#-installation)
  - [🎮 Using GeneticAlgo ](#-using-geneticalgo)
- [📄 License](#-license)

---


## 📍 Overview

This project focuses on solving the Assignment, Scheduling, and Routing Problems for employees within a SESSAD (Specialized Educational and Support Service at Home) setting. The project was undertaken as part of the academic curriculum, aiming to address the challenges of optimizing employee schedules and routes efficiently. A comprehensive examination of various operational research algorithms was conducted, leading to the selection of a Genetic Algorithm as the most suitable solution approach. The project involves the implementation of the Genetic Algorithm to effectively solve the given problem. Detailed insights into the research stages for algorithm selection, the coding structure of the Genetic Algorithm, and the parametrization steps are available in the "rapport (FR).pdf" file, which is written in French, as is the "Sujet (FR).pdf" file providing the project specifications.

---

## 🚀 Getting Started

### ✔️ Prerequisites

Before you begin, ensure that you have the following prerequisites installed:
> - `Make`
> - `C++ compiler`

### 📦 Installation

1. Clone the GeneticAlgo repository:
```sh
git clone https://github.com/simje71380/GeneticAlgo
```

2. Change to the project directory:
```sh
cd GeneticAlgo
```

3. Compiling:
```sh
make
```
or with a C++ compiler such as g++
```sh
g++ main.cpp chromosome.cpp ae.cpp population.cpp -o algo_evo
```

### 🎮 Using GeneticAlgo

```sh
./algo_evo <arguments>
```
arguments :

The application takes at least 4 arguments :<br />
-the file containing the characteristics of the participants in csv format<br />
-the file containing the characteristics of the missions in csv format<br />
-the file containing the distances in csv format<br />
-search time in seconds<br />

example :
```sh
./algo_evo Intervenants.csv Missions.csv Distance.csv 60
```

It is also possible to modify the algorithm parameters at launch by adding 4 extra arguments:<br />
-the number of generations<br />
-the probability of crossing<br />
-mutation probability<br />
-population size<br />

example :
```sh
./algo_evo Intervenants.csv Missions.csv Distance.csv 60 50000 0.8 0.5 50
```


## 📄 License

This project is licensed under the Apache License 2.0, which means you may not use this file except in compliance with the License. See the [LICENSE](./LICENSE) file for additional info

---