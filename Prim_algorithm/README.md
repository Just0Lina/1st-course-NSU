## Реализован алгоритм Прима

---

`Алгоритм нахождения каркаса графа`
### Вход:
#### строка 1:
	целое N от 0 до 5000 -- число вершин в графе
#### строка 2:
	целое M от 0 до N*(N-1)/2 -- число ребер в графе
#### от строки 3 до M+2:
	целое от 1 до N целое от 1 до N целое от 0 до INT_MAX -- начало, конец и длина ребра

### Выход:
	если N не от 0 до 5000, то "bad number of vertices"
	если M не от 0 до N*(N+1)/2, то "bad number of edges"
	если номер вершины не от 1 до N, то "bad vertex"
	если длина ребра не от 0 до INT_MAX, то "bad length"
	если строк меньше M+3, то "bad number of lines"