cbuffer LightCBuf
{
	float3 lightPos;
	float3 ambient;
	float3 diffuseColor;
	float diffuseIntensity;
	float attConst;
	float attLin;
	float attQuad;
};

cbuffer ObjectCBuf
{
	float3 materialColor;
	float specularIntensity;
	float specularPower;
}


float4 main(float3 worldPos : Position, float3 n : Normal) : SV_Target
{
	// ���C�g���_�f�[�^
	const float3 vToL = lightPos - worldPos;
	const float distToL = length(vToL);
	const float3 dirToL = vToL / distToL;
	// �g�U������
	const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
	// �g�U���x
	const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, n));
	// ���ˌ��x�N�g��
	const float3 w = n * dot(vToL, n);
	const float3 r = w * 2.0f - vToL;
	// ����x�N�g���Ɣ��˃x�N�g���̊p�x���狾�ʋ��x���v�Z
	const float3 specular = (diffuseColor * diffuseIntensity) * specularIntensity * pow(max(0.0f, dot(normalize(-r), normalize(worldPos))), specularPower);
	// �ŏI�I�ȐF
	return float4(saturate((diffuse + ambient + specular) * materialColor), 1.0f);
}