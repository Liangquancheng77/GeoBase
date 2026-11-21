// [D01] GeoBase 数值回归网
// 断言的是「正确行为」：改造前大面积红，改造后必须全绿。
// 参考解一律走独立数学路径（恒等变换 / 中心差分），不用另一份实现自证。
// 这两行必须放在最最最上面！
#define _CRT_SECURE_NO_WARNINGS 
#pragma execution_character_set("utf-8")
#include <gtest/gtest.h>

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "../include/GeoBase/Common.h"
#include "../include/GeoBase/Vector3.h"
#include "../include/GeoBase/Matrix4.h"
#include "../include/GeoBase/BezierCurve.h"
#include <cstdlib>   // std::getenv
#include <limits>    // std::numeric_limits


namespace {

    // 【类型别名】定义 4x4 的二维数组，用于在测试中方便地硬编码输入数据
    using Arr4 = double[4][4];

    // 【辅助函数】将一个 C 风格二维数组转换为自定义的 Matrix4 对象
    Matrix4 makeMat(const Arr4& v) {
        Matrix4 m;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                m.m[i][j] = v[i][j];
        return m;
    }

    // 【辅助函数】快速构造一个标准的 4x4 仿射变换矩阵（包含沿坐标轴的缩放和平移）
    Matrix4 makeAffine(double s0, double s1, double s2,
        double tx, double ty, double tz) {
        const Arr4 v = {
            { s0, 0.0, 0.0, tx },
            { 0.0, s1, 0.0, ty },
            { 0.0, 0.0, s2, tz },
            { 0.0, 0.0, 0.0, 1.0 }
        };
        return makeMat(v);
    }

    // 【辅助函数】计算两个矩阵乘积 (A*B) 与单位矩阵 (I) 之间的重构误差
    // 采用逐元素比较，返回绝对值最大的那个误差（即无穷范数意义下的误差）
    double reconError(const Matrix4& a, const Matrix4& b) {
        Matrix4 prod = a.multiply(b);
        double e = 0.0;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                e = std::max(e, std::abs(prod.m[i][j] - (i == j ? 1.0 : 0.0)));
        return e;
    }

    // 【辅助函数】计算两个三维向量在各个坐标分量上的最大绝对差值
    double maxAbsDiff(const Vector3& a, const Vector3& b) {
        return std::max({ std::abs(a.x - b.x),
                          std::abs(a.y - b.y),
                          std::abs(a.z - b.z) });
    }

    // 【辅助函数】使用“中心差分法”数值计算曲线在参数 t 处的导数
    // 这是一个独立于库中解析导数的参考解，用于防止“用错误代码测试错误代码”
    Vector3 centralDiff(const BezierCurve& c, double t, double h = 1e-5) {
        Point3 lo = c.evaluate(t - h);
        Point3 hi = c.evaluate(t + h);
        return (hi - lo) / (2.0 * h);
    }

    // 【辅助函数】返回一组固定坐标的三次贝塞尔曲线控制点，用于统一测试基准
    std::vector<Point3> bezierControlPoints() {
        return { Point3(0, 0, 0), Point3(1, 2, 0),
                 Point3(3, 0, 1), Point3(4, 1, 1) };
    }

    // 【辅助函数】将测试运行得到的各项指标（如误差值）记录到 CSV 文件
    // 仅在环境变量 GEABASE_NUMERIC_CSV 被设置时生效，用于生成对比报告
    void logMetric(const std::string& caseId, const std::string& scale,
        const std::string& metric, double value, bool pass,
        const std::string& note = "") {
        const char* path = std::getenv("GEABASE_NUMERIC_CSV");
        if (!path || !*path) return;
        static bool headerWritten = false;
        std::ofstream f(path, std::ios::app);
        if (!f) return;
        if (!headerWritten) {
            f << "case_id,scale,metric_name,metric_value,pass,note\n";
            headerWritten = true;
        }
        f << caseId << "," << scale << "," << metric << ","
            << value << "," << (pass ? 1 : 0) << "," << note << "\n";
    }

} // namespace

// ==================================================================
// A. 矩阵求逆
// ==================================================================

// 【测试用例】单位矩阵求逆往返测试：验证 I * I^-1 是否无限接近 I
TEST(Matrix4Inverse, IdentityRoundTrip) {                 // M4_INV_IDENTITY_01
    Matrix4 I;
    Matrix4 inv = I.inverse();
    double e = reconError(I, inv);
    logMetric("M4_INV_IDENTITY_01", "1", "recon_error", e, e < 1e-12);
    EXPECT_LT(e, 1e-12);
}

// 【测试用例】良态仿射矩阵求逆往返测试：验证正常缩放和平移矩阵求逆的精度
TEST(Matrix4Inverse, WellConditionedRoundTrip) {         // M4_INV_WELL_01
    // A = diag(2,3,4,1) + 平移(1,2,3)，det = 24，cond 小
    Matrix4 A = makeAffine(2.0, 3.0, 4.0, 1.0, 2.0, 3.0);
    ASSERT_NO_THROW({
        Matrix4 inv = A.inverse();
        double e = reconError(A, inv);
        logMetric("M4_INV_WELL_01", "1", "recon_error", e, e < 1e-12);
        EXPECT_LT(e, 1e-12);
        // 手算逆矩阵第3列应为 (-0.5, -2/3, -0.75, 1)
        EXPECT_NEAR(inv.m[0][3], -0.5, 1e-12);
        EXPECT_NEAR(inv.m[2][3], -0.75, 1e-12);
        });
}

// 【测试用例】奇异矩阵求逆异常测试1：验证输入含全零行的矩阵时能正确抛出异常
TEST(Matrix4Inverse, SingularZeroRowThrows) {            // M4_SINGULAR_01
    // 第 2 行全零 → 严格奇异，det 精确为 0
    const Arr4 v = {
        { 1.0, 2.0, 0.0, 1.0 },
        { 0.0, 1.0, 3.0, 2.0 },
        { 0.0, 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0, 1.0 }
    };
    Matrix4 A = makeMat(v);
    EXPECT_THROW({ (void)A.inverse(); }, std::runtime_error);
}

// 【测试用例】奇异矩阵求逆异常测试2：验证输入含重复行的矩阵时能正确抛出异常
TEST(Matrix4Inverse, SingularDuplicateRowsThrows) {     // M4_SINGULAR_02
    // 第 0、1 行相同 → 行线性相关，det 的浮点结果约 1e-16 量级
    const Arr4 v = {
        { 1.0, 2.0, 3.0, 4.0 },
        { 1.0, 2.0, 3.0, 4.0 },
        { 2.0, 1.0, 0.0, 1.0 },
        { 0.0, 0.0, 0.0, 1.0 }
    };
    Matrix4 A = makeMat(v);
    EXPECT_THROW({ (void)A.inverse(); }, std::runtime_error);
}

// 【测试用例】奇异矩阵求逆异常测试3：验证输入含线性相关行（A+B=C）的矩阵时能正确抛出异常
TEST(Matrix4Inverse, SingularDependentRowThrows) {      // M4_SINGULAR_03
    // 第 2 行 = 第 0 行 + 第 1 行 → 线性相关
    const Arr4 v = {
        { 1.0, 0.0, 2.0, 1.0 },
        { 0.0, 1.0, 1.0, 2.0 },
        { 1.0, 1.0, 3.0, 3.0 },
        { 0.0, 0.0, 0.0, 1.0 }
    };
    Matrix4 A = makeMat(v);
    EXPECT_THROW({ (void)A.inverse(); }, std::runtime_error);
}

// 【测试用例】非法输入异常测试：验证矩阵包含 NaN 等非有限值时能拦截并抛出异常
TEST(Matrix4Inverse, NonFiniteThrows) {                 // M4_SINGULAR_04
    // NaN 输入：std::abs(NaN) < 1e-12 为 false，改造前会继续算 1/NaN
    Matrix4 A = makeAffine(1.0, 1.0, 1.0, 0.0, 0.0, 0.0);
    A.m[1][1] = std::numeric_limits<double>::quiet_NaN();
    EXPECT_THROW({ (void)A.inverse(); }, std::runtime_error);
}

// ==================================================================
// B. Bezier 升阶不变性
// ==================================================================

// 【测试用例】二次贝塞尔升阶不变性：验证 3 个控制点升为 4 个后，曲线形状和端点不变
TEST(BezierElevate, QuadraticInvariance) {              // BEZ_ELEV_01
    std::vector<Point3> pts = {
        Point3(0, 0, 0), Point3(1, 2, 0), Point3(3, 0, 1)
    };
    BezierCurve before(pts);
    BezierCurve after(pts);
    after.elevateDegree();

    EXPECT_EQ(after.controlPoints().size(), 4u);       // n=2 → 3 个变 4 个
    for (double t : { 0.0, 0.25, 0.5, 0.75, 1.0 }) {
        double e = maxAbsDiff(before.evaluate(t), after.evaluate(t));
        logMetric("BEZ_ELEV_01", "1", "max_abs_err", e, e < 1e-12);
        EXPECT_LT(e, 1e-12) << "t = " << t;
    }
    // t=0.5 的闭式解，前后都应为 (1.25, 1.0, 0.25)

    //Point3 mid1 = before.evaluate(0.5);
    //EXPECT_NEAR(mid1.x, 1.25, 1e-12);
    //EXPECT_NEAR(mid1.y, 1.0, 1e-12);
    //EXPECT_NEAR(mid1.z, 0.25, 1e-12);

    Point3 mid2 = after.evaluate(0.5);
    EXPECT_NEAR(mid2.x, 1.25, 1e-12);
    EXPECT_NEAR(mid2.y, 1.0, 1e-12);
    EXPECT_NEAR(mid2.z, 0.25, 1e-12);
}

// 【测试用例】三次贝塞尔连续两次升阶不变性：验证 4 个控制点升为 6 个后，曲线形状依然不变
TEST(BezierElevate, CubicDoubleElevationInvariance) {  // BEZ_ELEV_02
    std::vector<Point3> pts = bezierControlPoints();
    BezierCurve before(pts);
    BezierCurve after(pts);
    after.elevateDegree();
    after.elevateDegree();

    EXPECT_EQ(after.controlPoints().size(), 6u);       // 4 → 5 → 6
    for (double t : { 0.0, 0.2, 0.5, 0.8, 1.0 }) {
        double e = maxAbsDiff(before.evaluate(t), after.evaluate(t));
        logMetric("BEZ_ELEV_02", "1", "max_abs_err", e, e < 1e-12);
        EXPECT_LT(e, 1e-12) << "t = " << t;
    }
}

// ==================================================================
// C. Bezier 导数
// ==================================================================

// 【测试用例】导数数值比对：验证解析求导的结果与中心差分法算出的数值解一致
TEST(BezierDerivative, MatchesCentralDifference) {      // BEZ_DERIV_01
    BezierCurve c(bezierControlPoints());
    const double t = 0.3;
    Vector3 analytic = c.derivative(t);
    Vector3 numeric = centralDiff(c, t);
    double e = maxAbsDiff(analytic, numeric);
    logMetric("BEZ_DERIV_01", "1", "max_abs_err", e, e < 1e-6);
    EXPECT_LT(e, 1e-6);
    // 解析值：3 * [0.49(P1-P0) + 0.42(P2-P1) + 0.09(P3-P2)] = (4.26, 0.69, 1.26)
    EXPECT_NEAR(analytic.x, 4.26, 1e-9);
    EXPECT_NEAR(analytic.y, 0.69, 1e-9);
    EXPECT_NEAR(analytic.z, 1.26, 1e-9);
}

// 【测试用例】端点导数验证：直接验证贝塞尔曲线在 t=0 和 t=1 处的切线（导数）是否正确
TEST(BezierDerivative, EndpointValues) {                // BEZ_DERIV_02
    BezierCurve c(bezierControlPoints());
    // C'(0) = 3(P1-P0) = (3,6,0)；C'(1) = 3(P3-P2) = (3,3,0)
    EXPECT_LT(maxAbsDiff(c.derivative(0.0), Vector3(3, 6, 0)), 1e-12);
    EXPECT_LT(maxAbsDiff(c.derivative(1.0), Vector3(3, 3, 0)), 1e-12);
}

// 【测试用例】线性曲线导数测试：一次贝塞尔曲线（直线）的导数处处相等，应为一个常量
TEST(BezierDerivative, LinearCurveConstant) {           // BEZ_DERIV_03
    // n=1 时导数应为常向量，且导数曲线恰好 1 个控制点
    std::vector<Point3> pts = { Point3(1, 1, 1), Point3(4, 5, 6) };
    BezierCurve c(pts);
    for (double t : { 0.0, 0.3, 0.7, 1.0 }) {
        double e = maxAbsDiff(c.derivative(t), Vector3(3, 4, 5));
        logMetric("BEZ_DERIV_03", "1", "max_abs_err", e, e < 1e-12);
        EXPECT_LT(e, 1e-12) << "t = " << t;
    }
}