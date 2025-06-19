# Assessment_expression_evaluation

Examples
```
Input:    add(1, 2, 3) → Output: 6
Input:    concat("a","b","c") → Output: abc
Input:    vector(1,2,3) → Output: [1, 2, 3]
Input:    range(3,6) → Output: [3, 4, 5]
Input:    addv(vector(1,2),vector(3,4)) → Output: [4, 6]
Input:    reduce_add(range(1,5)) → Output: 10
Input:    if_ge(5, 3, 100, 200) → Output: 100
Input:    if_ge(vector(1,3),vector(2,2),vector(10,10),vector(20,20)) → Output: [20, 10]

```

### Arithmetic
| Expression           | Output |
| -------------------- | ------ |
| `add(1, 2, 3, 4)`    | `10`   |
| `sub(10, 3)`         | `7`    |
| `add(sub(10, 5), 3)` | `8`    |
| `add(-1, -2, 3)`     | `0`    |


### Strings
| Expression                      | Output    |
| ------------------------------- | --------- |
| `concat("hi", "there")`         | `hithere` |
| `len("hello world")`            | `11`      |
| `parse("42")`                   | `42`      |
| `add(parse("4"), parse("5"))`   | `9`       |
| `concat("x", concat("y", "z"))` | `xyz`     |


### Vectors
| Expression                                   | Output         |
| -------------------------------------------- | -------------- |
| `vector(1, 2, 3)`                            | `[1, 2, 3]`    |
| `range(0, 4)`                                | `[0, 1, 2, 3]` |
| `addv(vector(1, 2), vector(3, 4))`           | `[4, 6]`       |
| `subv(vector(5, 5), vector(2, 3))`           | `[3, 2]`       |
| `reduce_add(vector(10, 20, 30))`             | `60`           |
| `reduce_add(addv(vector(1,2), vector(3,4)))` | `10`           |


### Conditionals with if_ge
| Expression                                                                | Output         |
| ------------------------------------------------------------------------- | -------------- |
| `if_ge(5, 3, 100, 200)`                                                   | `100`          |
| `if_ge(2, 3, 100, 200)`                                                   | `200`          |
| `if_ge(vector(5,2), vector(3,3), vector(1,1), vector(9,9))`               | `[1, 9]`       |
| `if_ge(range(1,4), vector(1,2,3), vector(10,20,30), vector(100,200,300))` | `[10, 20, 30]` |

### Nested & Mixed Types

| Expression                                                                | Output    |
| ------------------------------------------------------------------------- | --------- |
| `reduce_add(if_ge(range(1,4), range(1,4), vector(1,1,1), vector(9,9,9)))` | `3`       |
| `reduce_add(addv(range(1,4), range(1,4)))`                                | `12`      |
| `concat("sum:", parse("123"))`                                            | `sum:123` |

