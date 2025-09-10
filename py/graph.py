import matplotlib.pyplot as plt
import numpy as np
from matplotlib.colors import LinearSegmentedColormap

# Чтение данных из файла
misses = []
numOfKeys = []
keyDensity = []

with open('py/data.txt', 'r') as file:
    lines = file.readlines()
    # Пропускаем первую строку с заголовками
    for line in lines[1:]:
        data = line.strip().split()
        misses.append(float(data[0]))
        numOfKeys.append(float(data[1]))
        keyDensity.append(float(data[2]))

# Создаем кастомную цветовую карту от зеленого к красному
colors = ['green', 'yellow', 'red']
cmap = LinearSegmentedColormap.from_list('green_to_red', colors, N=100)

# Создаем 3D график
fig = plt.figure(figsize=(14, 10))
ax = fig.add_subplot(111, projection='3d')

# Отображаем точки с цветовой шкалой от зеленого к красному
scatter = ax.scatter(numOfKeys, keyDensity, misses, 
                    c=misses, cmap=cmap, s=150, alpha=0.8, edgecolors='black', linewidth=0.5)

# Настраиваем оси
ax.set_xlabel('Number of Keys', fontsize=12, fontweight='bold')
ax.set_ylabel('Key Density', fontsize=12, fontweight='bold')
ax.set_zlabel('Misses', fontsize=12, fontweight='bold')
ax.set_title('LIRS Cache Performance\n(Green = Low Misses, Red = High Misses)', 
             fontsize=14, fontweight='bold', pad=20)

# Добавляем цветовую шкалу
cbar = fig.colorbar(scatter, ax=ax, shrink=0.5, aspect=20, pad=0.1)
cbar.set_label('Misses Count', fontsize=12, fontweight='bold')
cbar.ax.tick_params(labelsize=10)

# Настраиваем вид графика
ax.grid(True, alpha=0.3)
ax.view_init(elev=20, azim=45)  # Угол обзора

# Показываем график
plt.tight_layout()
plt.show()