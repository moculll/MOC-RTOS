# MOC-RTOS User Guide
- Environment: Ubuntu22.04 Windows10
## 1. Clone MOC-RTOS
```
git clone https://github.com/moculll/MOC-RTOS.git
cd MOC-RTOS
```
## 2. Get cross compile tools
#### 2.1 decide which board you're going to use

```
vim ./config/boards.cfg


# boards.cfg, this is for getting necessary build essentials
BOARDS=[nrf52840dk]
```

or
```
BOARDS=[nrf52840dk, esp32h2]
```

#### 2.2 bootstarp for board dependency

```
# Linux
./scripts/bootstrap.sh
```

```
# Windows
.\scripts\bootstrap.bat
```

## 3. Compile the project

```
# Linux
./scripts/build.sh BOARD=nrf52840dk
```

```
# Windows
.\scripts\build.bat BOARD=nrf52840dk
```

## 4. Flash the executable file into your Board and Enjoy!

```
# Linux
nrfjprog -f nrf52 --sectorerase --program out/nrf52840dk/MOC-RTOS.hex --reset
```




```
# Windows
nrfjprog -f nrf52 --sectorerase --program out\nrf52840dk\MOC-RTOS.hex --reset
```
```
Note that currently in Windows we are not able to download segger Jlink driver and
nrfjprog for you, you need to download them manually to flash the board
```
# Contribute
- #### Commit Fomat
```
[date][author](type): changes
```
e.g. 
```
[2024-06-22][mocul](optimize/fix/addFunc): 
1. remove useless files
2. fix build issue
3. add more makefiles to board nrf52840dk
```

- #### FIXME
```
current project contains multiple of "FIXME" in some files
these "FIXME" need to be fixed, we can simply search them by coding tools

after fixing, we recommend to commit by template as below:
[2024-06-22][mocul](FIXME): 
1. fix thread pid function

```

- #### BackupCommits

```
assume that we have two branches "main" and "working_on_thread_porting"
if we're in branch "working_on_thread_porting", we can push a backup commit as below:

[date][author](branch)(type): changes
```
e.g.
```
[2024-06-22][mocul](working_on_thread_porting)(BACKUP):
1. working on thread porting, backup
```

```
and we need to remember to add all changes in the future commits
```

e.g.

```
[2024-06-22][mocul](optimize/fix/addFunc):
1. added thread mutex
2. added thread msg function

```