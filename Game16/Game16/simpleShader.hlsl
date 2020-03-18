////��b�̋@�\������shader
//
//
////�O���[�o���ϐ�
//cbuffer global
//{
//	matrix g_WVP;  //���[���h����ˉe�܂ł̕ϊ��s��
//};
//
////�o�[�e�b�N�X�V�F�[�_�[�i���_���j
////�Z�}���e�B�N�XPOSITION
//float4 VS(float4 Pos:POSITION):SV_POSITION
//{
//	//�ʒu��mul�֐����g�p����World���W�𒲐�
//	Pos = mul(Pos,g_WVP);
//    return Pos;
//}
////�s�N�Z���V�F�[�_�[�i�s�N�Z���P�ʂŎw��͈͓����ǂ̐F�ŕ`�悷�邩�j
//float4 PS(float4 Pos:SV_POSITION) :SV_Target
//{
//	//����App������s�N�Z���������炤���Ƃ��o���Ȃ��̂ŁA��x�o�[�e�b�N�X�V�F�[�_�[���o�R����
//	//����������炤
//	//��芸�������ŕ`��
//	return float4(1,1,1,1);
//}


Texture2D g_texColor: register(t0);
SamplerState g_samLinear : register(s0);

cbuffer global
{
	matrix g_mWVP;//���[���h����ˉe�܂ł̕ϊ��s��
};

//�\����
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;    //UV���W�������Ă���
};

//
//�o�[�e�b�N�X�V�F�[�_�[
//
VS_OUTPUT VS(float4 Pos : POSITION, float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(Pos, g_mWVP);
	output.Tex = Tex;   //�����Ă���UV���W���s�N�Z���V�F�[�_�[�ɓn��

	return output;
}

//
//�s�N�Z���V�F�[�_�[
//
float4 PS(VS_OUTPUT input) : SV_Target
{
	//�����Ă���UV���W
	return g_texColor.Sample(g_samLinear, input.Tex);
}