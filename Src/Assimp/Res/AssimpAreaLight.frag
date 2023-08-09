#version 330 core

out vec4 FlagColor;

in vec3 worldPosition;
in vec3 worldNormal;
in vec2 texcoord;

struct Light
{
	float intensity;
	vec3 color;
	vec3 point[4];
	bool twoSided;
};

uniform Light areaLight;
uniform vec3 areaLightTranslate;

struct Material
{
	sampler2D diffuse;
	vec4 albedoRoughness; //(x,y,z) = color, w = roughness （余剰分）
};
uniform Material material;

uniform vec3 viewPosition;
uniform sampler2D LTC1; //逆マトリックス
uniform sampler2D LTC2; //鏡面反射　周波数　0（未使用）球

const float LUT_SIZE = 64.0f; //カラーグレーディングのサイズ
const float LUT_SCALE = (LUT_SIZE - 1.0) / LUT_SIZE;
const float LUT_BIAS = 0.5/LUT_SIZE;//カラーグレーディングの傾向

//平面への投影のないベクトル形式
//プロキシ球クリッピングに使用
vec3 IntegrateEdgeVec(vec3 v1,vec3 v2)
{
	//組み込みのacos()関数を使用すると、欠陥が発生する
	//acos()の計算にフィッティング結果を使用する
	float x = dot(v1,v2);
	float y = abs(x);

	float a = 0.8543985 + (0.4965155 + 0.0145206 * y) * y;
	float b = 3.4175940 + (4.1616724 + y) * y;
    float v = a / b;

	//平方根の逆数
    float theta_sintheta = (x > 0.0) ? v : 0.5 * inversesqrt(max(1.0 - x*x, 1e-7)) - v;

	return cross(v1, v2) * theta_sintheta;
}

float IntegrateEdge(vec3 v1,vec3 v2)
{
	return IntegrateEdgeVec(v1,v2).z;
}

//returnはワールド空間のフラグポジション(LTC分布)
vec3 LTC_Evaluate(vec3 N,vec3 V,vec3 P,mat3 minv,vec3 point[4],bool twoSided)
{
	//N の周りの正規直交基底を構築する
	vec3 T1,T2;
	T1 = normalize(V - N * dot(V,N));
	T2 = cross(N,T1);

	//エリアライトを(T1、T2、N)ベースで回転させる
	minv = minv * transpose(mat3(T1,T2,N));

	//ポリゴン (クリッピング用に 4 つの頂点を割り当てる)
	vec3 L[4];
	//ポリゴンを LTC から原点に戻す変換(コサイン加重)
	L[0] = minv * (point[0] - P);
	L[1] = minv * (point[1] - P);
	L[2] = minv * (point[2] - P);
	L[3] = minv * (point[3] - P);

	//表形式の水平方向にクリップされた球を使用する
	//シェーディングポイントがライトの後ろにあるかどうかを確認
	vec3 dir = point[0] - P; //LTCの場所
	vec3 lightNormal = cross(point[1] - point[0],point[3] - point[0]);
	bool behind = (dot(dir,lightNormal) < 0.0);

	//コサインの値を入れる
	L[0] = normalize(L[0]);
	L[1] = normalize(L[1]);
	L[2] = normalize(L[2]);
	L[3] = normalize(L[3]);

	//統合
	vec3 vsum = vec3(0.0);
	vsum += IntegrateEdgeVec(L[0],L[1]);
	vsum += IntegrateEdgeVec(L[1],L[2]);
	vsum += IntegrateEdgeVec(L[2],L[3]);
	vsum += IntegrateEdgeVec(L[3],L[0]);

	//方向vsumのポリゴンの規格
	float len = length(vsum);

	float z = vsum.z / len;
	if(behind)
		z = -z;

	vec2 uv = vec2(z * 0.5 + 0.5,len); //幅 0,1
	uv = uv * LUT_SCALE + LUT_BIAS;

	//水平方向のクリッピングの規格を一致させる
	float scale = texture(LTC2,uv).w;

	float sum = len * scale;
	if(!behind && !twoSided)
		sum = 0.0f;
	
	//ポリゴン全体の出力放射輝度(立体角)
	vec3 LoI = vec3(sum,sum,sum);
	return LoI;
}

vec3 PowVec3(vec3 v,float p)
{
	return vec3(pow(v.x,p),pow(v.y,p),pow(v.z,p));
}

//画像の階調の応答特性を示す数値
const float gamma = 2.2;
//線形
vec3 ToLinear(vec3 v){return PowVec3(v,gamma);}

vec3 ToSRGB(vec3 v){return PowVec3(v,1.0/gamma);}

void main()
{
	//ガンマ補正
	vec3 mDiffuse = texture(material.diffuse,texcoord).xyz;//理論値 (0.7,0.8,0.96)
	vec3 mSpecular = ToLinear(vec3(0.23f,0.23f,0.23f));

	vec3 result = vec3(0.0f);

	vec3 N = normalize(worldNormal);
	vec3 V = normalize(viewPosition - worldPosition);
	vec3 P = worldPosition;
	float dotNV = clamp(dot(N,V),0.0f,1.0f);

	//粗さとsqrt(1-cos_theta)を使用してM_textureをサンプリング
	vec2 uv = vec2(material.albedoRoughness.w , sqrt(1.0f - dotNV));
	uv = uv * LUT_SCALE + LUT_BIAS;

	//逆マトリクスの4つのパラメータを取得する
	vec4 t1 = texture(LTC1,uv);

	//フレネル計算の2つのパラメータを取得する
	vec4 t2 = texture(LTC2,uv);

	mat3 Minv = mat3(
	vec3(t1.x,0,t1.y),
	vec3(0,   1,0),
	vec3(t1.z,0,t1.w)
	);

	/*テスト用の光源の翻訳*/
	vec3 translatedPoints[4];
	translatedPoints[0] = areaLight.point[0] + areaLightTranslate;
    translatedPoints[1] = areaLight.point[1] + areaLightTranslate;
    translatedPoints[2] = areaLight.point[2] + areaLightTranslate;
    translatedPoints[3] = areaLight.point[3] + areaLightTranslate;

	//LTCシェーディングをする
	vec3 diffuse = LTC_Evaluate(N,V,P,mat3(1),translatedPoints,areaLight.twoSided);
	vec3 specular = LTC_Evaluate(N,V,P,Minv,translatedPoints,areaLight.twoSided);

	//シャドウイングとフレネル
	/*
	t2.x : シャドウフレネル90(F90は通常1.0である必要がある)
	t2.y : 幾何減衰項のスミス関数は、ドット(VまたはL、H)です。
	*/
	specular *= mSpecular * t2.x + (1.0f - mSpecular) *  t2.y;

	result = areaLight.color * areaLight.intensity * (specular + mDiffuse * diffuse);

	FlagColor = vec4(ToSRGB(result),1.0f);
}