Voici une **version intégralement basculée en anglais**, propre et cohérente, sans changer le fond (juste la langue et quelques formulations rendues plus naturelles).

---

````md
# embedded-sys-energy

## Links
- [Project description – Da Costa](https://www.irit.fr/~Georges.Da-Costa/syst%C3%A8mes-embarqu%C3%A9s-%C3%A9nergie/)
- [Moodle – Embedded Systems, Energy](https://moodle.utoulouse.fr/course/view.php?id=5203#Energie)

## Connection
```bash
ssh ileginyo@access.grid5000.fr
````

```bash
ssh nancy
```

```bash
oarsub -I -l walltime=1:0:0 -p "cluster='gros'"
```

## Useful commands

```bash
sudo-g5k
```

> used to switch to root privileges

## MojitO/S

> RAPL sensor: memory and CPU energy consumption (in micro-joules)

* `f` | frequency: set the number of measurements per second
* `t` | time: set the execution duration (seconds). If 0, runs indefinitely
* `o` | option: specify a log file for MojitO/S or a port number for prometheus_mojitO/S
  (`<output file>` or `<port number>`)
* `s` | overhead-stats: enable overhead statistics (nanoseconds)

## Commit Message Rules

### Format

```
<type>: <short summary>
```

### Where

**type** must be one of:

* `feat` → new feature
* `fix` → bug fix
* `refactor` → code restructuring without behavior change
* `docs` → documentation
* `build` → build system / Makefile / CI changes

**short summary** → concise, imperative description
(recommended maximum: ~50 characters)

### Example

```
feat: add timestamp-based automatic CSV file naming
```

### Additional rules

1. **Use imperative tense**: “Add”, “Fix”, “Remove”
2. **Keep the summary short and descriptive** (details go in the body)
3. **Optionally**, add a commit body after a blank line if more context is needed

```
