# embedded-sys-energy

## Liens
- [Sujet - Da Costa](https://www.irit.fr/~Georges.Da-Costa/syst%C3%A8mes-embarqu%C3%A9s-%C3%A9nergie/)

- [Moodle - Systèmes embarqués, Energie](https://moodle.utoulouse.fr/course/view.php?id=5203#Energie)



## Connexion
```
ssh ileginyo@access.grid5000.fr
```

```
ssh nancy
```

```
oarsub -I -l walltime=1:0:0 -p "cluster='gros'"
```

## Commandes utiles
```
sudo-g5k
```
> pour passer en root

## MojitO/S
> capteur RAPL : Consommation mémoire et processeur (en micro-joules)

- f | frequence : set amount of measurements per second
- t | time : set duration value (seconds). If 0, then loops infinitely.
- o | option : specify a log file for MojitO/S or a port number for prometeus_mojitO/S. \<output file> or \<port number>.
- s | overhead-stats : enable overhead statistics (nanoseconds).


### **Commit Message Rule **

**Format:**

```
<type>: <short summary>
```

**Where:**

* **type** is one of:

  * `feat` → new feature
  * `fix` → bug fix
  * `refactor` → code restructuring without changing behavior
  * `docs` → documentation
  * `build` → build system / Makefile / CI changes
* **short summary** → concise, imperative-style description (max ~50 chars recommended)

**Example:**

```
feat: add timestamp-based automatic CSV file naming
```

**Additional rules:**

1. **Imperative tense**: “Add”, “Fix”, “Remove” (like giving a command).
2. **Keep the subject short and descriptive** (don’t include details in the summary).
3. **Optionally** add a body after a blank line for more explanation if needed.