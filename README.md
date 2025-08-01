# Elimination-tree-decomposition

A heuristic tree decomposition algorithm. First a tree decomposition is build using vertex elimination orderings,
where an addressable priority queue determines the order based on a greedy approach.

For non-simplicial vertices the key value is given by edge density times bagsize()-1/bagsize 


## Command-Line Usage

```bash
elimination [options] <.td file> <vertex>
```

### Arguments

|Argument                   |Description                                                | Required  |
|---------------------------|-----------------------------------------------------------|-----------|
|`<.graph file>`            | Path to input graph file                                  |   yes     |
|`<vertex>`                 | Provides an element of randomness for a key               |   yes     |

### Options

| Option                      | Description                                                                                 | Default |
|-----------------------------|---------------------------------------------------------------------------------------------|---------|
| `--print_td=true\|false` | Print full tree decomposition                                                     | false   |

## Example

<details>
<summary>data/test_set/mytest.graph</summary>

```
7 9 10
10 2 7
9 1 3 4
8 2 6 7
7 2 5
6 4 6
5 3 5 7
4 1 3 6
```

</details>

<details>
<summary>data/test_set/mytest.td</summary>

```
s td 7 4 7
b 1 1 2 7 
b 2 
b 3 
b 4 
b 5 2 4 5 
b 6 2 5 6 
b 7 2 3 6 7 
1 7
2 3
2 4
4 5
5 6
6 7
```

</details>