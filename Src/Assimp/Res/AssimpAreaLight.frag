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
	vec4 albedoRoughness; //(x,y,z) = color, w = roughness �i�]�蕪�j
};
uniform Material material;

uniform vec3 viewPosition;
uniform sampler2D LTC1; //�t�}�g���b�N�X
uniform sampler2D LTC2; //���ʔ��ˁ@���g���@0�i���g�p�j��

const float LUT_SIZE = 64.0f; //�J���[�O���[�f�B���O�̃T�C�Y
const float LUT_SCALE = (LUT_SIZE - 1.0) / LUT_SIZE;
const float LUT_BIAS = 0.5/LUT_SIZE;//�J���[�O���[�f�B���O�̌X��

//���ʂւ̓��e�̂Ȃ��x�N�g���`��
//�v���L�V���N���b�s���O�Ɏg�p
vec3 IntegrateEdgeVec(vec3 v1,vec3 v2)
{
	//�g�ݍ��݂�acos()�֐����g�p����ƁA���ׂ���������
	//acos()�̌v�Z�Ƀt�B�b�e�B���O���ʂ��g�p����
	float x = dot(v1,v2);
	float y = abs(x);

	float a = 0.8543985 + (0.4965155 + 0.0145206 * y) * y;
	float b = 3.4175940 + (4.1616724 + y) * y;
    float v = a / b;

	//�������̋t��
    float theta_sintheta = (x > 0.0) ? v : 0.5 * inversesqrt(max(1.0 - x*x, 1e-7)) - v;

	return cross(v1, v2) * theta_sintheta;
}

float IntegrateEdge(vec3 v1,vec3 v2)
{
	return IntegrateEdgeVec(v1,v2).z;
}

//return�̓��[���h��Ԃ̃t���O�|�W�V����(LTC���z)
vec3 LTC_Evaluate(vec3 N,vec3 V,vec3 P,mat3 minv,vec3 point[4],bool twoSided)
{
	//N �̎���̐��K���������\�z����
	vec3 T1,T2;
	T1 = normalize(V - N * dot(V,N));
	T2 = cross(N,T1);

	//�G���A���C�g��(T1�AT2�AN)�x�[�X�ŉ�]������
	minv = minv * transpose(mat3(T1,T2,N));

	//�|���S�� (�N���b�s���O�p�� 4 �̒��_�����蓖�Ă�)
	vec3 L[4];
	//�|���S���� LTC ���猴�_�ɖ߂��ϊ�(�R�T�C�����d)
	L[0] = minv * (point[0] - P);
	L[1] = minv * (point[1] - P);
	L[2] = minv * (point[2] - P);
	L[3] = minv * (point[3] - P);

	//�\�`���̐��������ɃN���b�v���ꂽ�����g�p����
	//�V�F�[�f�B���O�|�C���g�����C�g�̌��ɂ��邩�ǂ������m�F
	vec3 dir = point[0] - P; //LTC�̏ꏊ
	vec3 lightNormal = cross(point[1] - point[0],point[3] - point[0]);
	bool behind = (dot(dir,lightNormal) < 0.0);

	//�R�T�C���̒l������
	L[0] = normalize(L[0]);
	L[1] = normalize(L[1]);
	L[2] = normalize(L[2]);
	L[3] = normalize(L[3]);

	//����
	vec3 vsum = vec3(0.0);
	vsum += IntegrateEdgeVec(L[0],L[1]);
	vsum += IntegrateEdgeVec(L[1],L[2]);
	vsum += IntegrateEdgeVec(L[2],L[3]);
	vsum += IntegrateEdgeVec(L[3],L[0]);

	//����vsum�̃|���S���̋K�i
	float len = length(vsum);

	float z = vsum.z / len;
	if(behind)
		z = -z;

	vec2 uv = vec2(z * 0.5 + 0.5,len); //�� 0,1
	uv = uv * LUT_SCALE + LUT_BIAS;

	//���������̃N���b�s���O�̋K�i����v������
	float scale = texture(LTC2,uv).w;

	float sum = len * scale;
	if(!behind && !twoSided)
		sum = 0.0f;
	
	//�|���S���S�̂̏o�͕��ˋP�x(���̊p)
	vec3 LoI = vec3(sum,sum,sum);
	return LoI;
}

vec3 PowVec3(vec3 v,float p)
{
	return vec3(pow(v.x,p),pow(v.y,p),pow(v.z,p));
}

//�摜�̊K���̉����������������l
const float gamma = 2.2;
//���`
vec3 ToLinear(vec3 v){return PowVec3(v,gamma);}

vec3 ToSRGB(vec3 v){return PowVec3(v,1.0/gamma);}

void main()
{
	//�K���}�␳
	vec3 mDiffuse = texture(material.diffuse,texcoord).xyz;//���_�l (0.7,0.8,0.96)
	vec3 mSpecular = ToLinear(vec3(0.23f,0.23f,0.23f));

	vec3 result = vec3(0.0f);

	vec3 N = normalize(worldNormal);
	vec3 V = normalize(viewPosition - worldPosition);
	vec3 P = worldPosition;
	float dotNV = clamp(dot(N,V),0.0f,1.0f);

	//�e����sqrt(1-cos_theta)���g�p����M_texture���T���v�����O
	vec2 uv = vec2(material.albedoRoughness.w , sqrt(1.0f - dotNV));
	uv = uv * LUT_SCALE + LUT_BIAS;

	//�t�}�g���N�X��4�̃p�����[�^���擾����
	vec4 t1 = texture(LTC1,uv);

	//�t���l���v�Z��2�̃p�����[�^���擾����
	vec4 t2 = texture(LTC2,uv);

	mat3 Minv = mat3(
	vec3(t1.x,0,t1.y),
	vec3(0,   1,0),
	vec3(t1.z,0,t1.w)
	);

	/*�e�X�g�p�̌����̖|��*/
	vec3 translatedPoints[4];
	translatedPoints[0] = areaLight.point[0] + areaLightTranslate;
    translatedPoints[1] = areaLight.point[1] + areaLightTranslate;
    translatedPoints[2] = areaLight.point[2] + areaLightTranslate;
    translatedPoints[3] = areaLight.point[3] + areaLightTranslate;

	//LTC�V�F�[�f�B���O������
	vec3 diffuse = LTC_Evaluate(N,V,P,mat3(1),translatedPoints,areaLight.twoSided);
	vec3 specular = LTC_Evaluate(N,V,P,Minv,translatedPoints,areaLight.twoSided);

	//�V���h�E�C���O�ƃt���l��
	/*
	t2.x : �V���h�E�t���l��90(F90�͒ʏ�1.0�ł���K�v������)
	t2.y : �􉽌������̃X�~�X�֐��́A�h�b�g(V�܂���L�AH)�ł��B
	*/
	specular *= mSpecular * t2.x + (1.0f - mSpecular) *  t2.y;

	result = areaLight.color * areaLight.intensity * (specular + mDiffuse * diffuse);

	FlagColor = vec4(ToSRGB(result),1.0f);
}