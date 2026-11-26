import pandas as pd
import matplotlib.pyplot as plt

# 1. 设置中文字体支持（Windows 常用 'SimHei' 或 'Microsoft YaHei'）
plt.rcParams['font.sans-serif'] = ['SimHei']  # 用来正常显示中文标签
plt.rcParams['axes.unicode_minus'] = False    # 用来正常显示负号（否则轴上的负数会变成方块）

# 读取 C++ 生成的数据
try:
    df = pd.read_csv('interpolation_compare.csv')
except FileNotFoundError:
    print("Error: interpolation_compare.csv not found. Please run the C++ program first.")
    exit(1)

plt.figure(figsize=(12, 6))

# 绘制真实函数
plt.plot(df['x'], df['true_value'], 'k--', label='真实函数 (龙格)', linewidth=2)

# 绘制等距节点插值 (红色，大概率会在两端震荡飞出)
plt.plot(df['x'], df['equidistant'], 'r-', label='等距节点插值 (n=11)', linewidth=2)

# 绘制切比雪夫节点插值 (蓝色，应该紧紧贴合黑虚线)
plt.plot(df['x'], df['chebyshev'], 'b-', label='切比雪夫节点插值 (n=11)', linewidth=2)

# 设置图表格式
plt.ylim(-1, 2)  # 限制 y 轴范围，否则等距节点的龙格现象会飞出屏幕导致看不清
plt.title('插值效果对比：等距节点 vs 切比雪夫 (n=11)', fontsize=14)
plt.xlabel('x', fontsize=12)
plt.ylabel('y', fontsize=12)
plt.legend(fontsize=12)
plt.grid(True, linestyle='--', alpha=0.6)

plt.tight_layout()
plt.show()