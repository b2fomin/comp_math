Для запуска приложения использовать файл .exe. Вводите в первые две ячейки
таблицы координаты точек x и y соответственно. После нажатия кнопки Calculate
появятся еще 4 колонки, в которых будут отображены коэффициенты при члене нулевой,
первой, второй и третьей степени соответственно напротив точки, являющейся
концом интервала, в котором определен данный сплайн. При нажатии на кнопку
Show Graph отобразится график сплайна (возможно, потребуется поменять размер
окна для его корректного отображения). При нажатии на кнопку Clear Table таблица
очищается.

Проверяющему требуется только просмотреть на код в файлах spline_calc.h и spline_calc.cpp.
Функция system_solving решает систему уравнений для коэффициентов сплайна при второй
степени методом прогонки, так как таблица трехдиагональная. Функция spline получает СЛАУ
для коэффицентов при второй степени и вычисляет остальные.