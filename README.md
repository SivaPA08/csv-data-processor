CSV Data Processor which evaluates and process data faster by using OpenMP

# csv-data-processor

A lightweight CLI tool to **query, aggregate, and update numeric CSV files** directly from the terminal.

* Automatically loads `file.csv` from the working directory
* First row = column names
* Remaining rows = numeric data (`double`)
* Commands can be chained using `|` (pipe operator)
* Uses OpenMP for fast execution

When started, it prints the list of columns and shows a prompt:

```
Welcome to csv-data-processor
csv-data-processor>>>
```

---

## Supported Commands

### Aggregate Range — `a(...)`

```
a(<column>, <start>, <end>)
```

Sums values in a column from row `start` to `end`.
Use `*` to mean full coverage.

Examples:

```
a(SALARY,0,99)
a(PRICE,*,*)
```

---

### Conditional Aggregate — `ca(...)`

```
ca(<column>, <condition>)
```

Filters cells based on a condition and sums the matching ones.
`x` refers to the current cell value.

Examples:

```
ca(SALARY, x>50000)
ca(AGE, x>=18 && x<=60)
```

---

### Update Range — `u(...)`

```
u(<column>, <start>, <end>, <expression-or-value>)
```

Updates values in a given range.

* Expression uses `x` as the old value
* Or assign constant values

Examples:

```
u(SALARY,0,*, 50000)
u(AGE,*,*, x+1)
```

---

### Conditional Update — `cu(...)`

```
cu(<column>, <condition>, <expression-or-value>)
```

Only updates rows where the condition evaluates true.

Examples:

```
cu(SALARY, x<30000, x*1.2)
cu(AGE, x<18, 18)
```

---

### Show Columns — `s(...)`

```
s(<col1 col2 col3>, <start>, <end>)
```

Displays selected columns in table format.

Example:

```
s(ID NAME SALARY,*,*)
```

> Note: Current version prints entire dataset ignoring range args.

---

### Save and Exit — `commit`

```
commit
```

Saves data back to `file.csv` and ends program.

---

### Exit Without Saving — `exit`

```
exit
```

Quits and discards unsaved changes.

---

## Result Output + Timer

After running commands (single or chained), tool prints:

* Execution Time (in seconds)
* Compact result table:

  * 1st row: commands executed
  * 2nd row: result per command
    (`sum`, `Null` for update, `Error` if failed)

---

## Build & Run

Ensure a C++ compiler with OpenMP is available.


Run:

```bash
./parallel.sh
```

Your `file.csv` must be placed in the executable directory.

---

## Example CSV Format

```
ID,AGE,SALARY
1,22,40000
2,35,65000
3,29,52000
```

---

## Feature Summary

| Feature                 | Status |
| ----------------------- | :----: |
| Range aggregation       |        |
| Conditional aggregation |        |
| Range update            |        |
| Conditional update      |        |
| Parallel execution      |        |
| Save back to CSV        |        |
| Table output            |        |
| Command chaining        |        |

---

## Purpose

Provides a fast and simple way to manipulate numeric CSV datasets without needing SQL or heavy spreadsheet tools — ideal for bulk analytics and quick data experiments.

---


<img width="890" height="854" alt="Image" src="https://github.com/user-attachments/assets/ecf8e7c1-273f-41cf-a0cf-b7b6038e526a" />

