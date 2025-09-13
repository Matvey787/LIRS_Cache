import matplotlib.pyplot as plt
import numpy as np
from matplotlib.colors import LinearSegmentedColormap

# Устанавливаем темный фон для всего графика
plt.style.use('dark_background')

# Чтение данных из файлов
misses_lirs = []
numOfKeys_lirs = []
keyDensity_lirs = []

misses_opt = []
numOfKeys_opt = []
keyDensity_opt = []

# Чтение данных LIRS
with open('py/cacheData.txt', 'r') as file:
    lines = file.readlines()
    for line in lines[1:]:
        data = line.strip().split()
        misses_lirs.append(float(data[1]) - float(data[0]))
        numOfKeys_lirs.append(float(data[1]))
        keyDensity_lirs.append(float(data[2]))

# Чтение данных OPT
with open('py/OPTCacheData.txt', 'r') as file:
    lines = file.readlines()
    for line in lines[1:]:
        data = line.strip().split()
        misses_opt.append(float(data[1]) -float(data[0]))
        numOfKeys_opt.append(float(data[1]))
        keyDensity_opt.append(float(data[2]))

# Создаем кастомную цветовую карту от красного к зеленому
colors = ['green', 'yellow', 'red']
cmap = LinearSegmentedColormap.from_list('green_to_red', colors, N=100)

# Создаем 3D график с темным фоном
fig = plt.figure(figsize=(14, 10), facecolor='#1a1a1a')
ax = fig.add_subplot(111, projection='3d')

# Устанавливаем темный фон для области графика
ax.set_facecolor('#2a2a2a')

# Отображаем точки LIRS (более темные)
scatter_lirs = ax.scatter(numOfKeys_lirs, keyDensity_lirs, misses_lirs, 
                         c=misses_lirs, cmap=cmap, s=50, alpha=1, 
                         edgecolors='white', linewidth=0.5, label='LIRS')

# Отображаем точки OPT (более светлые)
scatter_opt = ax.scatter(numOfKeys_opt, keyDensity_opt, misses_opt, 
                        c=misses_opt, cmap=cmap, s=50, alpha=0.6, 
                        edgecolors='black', linewidth=0.5, label='OPT')

# Настраиваем оси с белым цветом текста
ax.set_xlabel('Number of Keys', fontsize=12, fontweight='bold', color='white')
ax.set_ylabel('Key Density', fontsize=12, fontweight='bold', color='white')
ax.set_zlabel('Misses', fontsize=12, fontweight='bold', color='white')
ax.set_title('Cache Performance Comparison\n(Red = HIGH Misses, Green = Low Misses)', 
             fontsize=14, fontweight='bold', pad=20, color='white')

# Настраиваем цвет осей
ax.xaxis.pane.fill = False
ax.yaxis.pane.fill = False
ax.zaxis.pane.fill = False
ax.xaxis.pane.set_edgecolor('#444444')
ax.yaxis.pane.set_edgecolor('#444444')
ax.zaxis.pane.set_edgecolor('#444444')

# Настраиваем цвет сетки
ax.grid(True, alpha=0.2, color='white')

# Добавляем вертикальную цветовую шкалу
cbar = fig.colorbar(scatter_lirs, ax=ax, shrink=0.5, aspect=20, pad=0.1)
cbar.set_label('Misses Count', fontsize=12, fontweight='bold', color='white')
cbar.ax.tick_params(labelsize=10, colors='white')

# Устанавливаем цвет фона для цветовой шкалы
cbar.ax.set_facecolor('#2a2a2a')

# Добавляем легенду с белым текстом
ax.legend(fontsize=12, facecolor='#2a2a2a', edgecolor='white', labelcolor='white')

# Настраиваем вид графика
ax.view_init(elev=20, azim=45)

# Показываем график
plt.tight_layout()
plt.show()