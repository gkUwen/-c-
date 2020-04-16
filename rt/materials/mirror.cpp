#include <rt/materials/mirror.h>
#include <core/scalar.h>
#include <core/point.h>

namespace rt {

    MirrorMaterial::MirrorMaterial(float eta, float kappa)
	{
		this->eta = eta;
		this->kappa = kappa;

		this->eta_kappa_sqr = (eta*eta) + (kappa*kappa);
		this->two_eta = 2 * eta;	
	}

	RGBColor MirrorMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const
	{
		return RGBColor::rep(0.f);
	}
	
	RGBColor MirrorMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const
	{
		return RGBColor::rep(0.f);
	}
	
	Material::SampleReflectance MirrorMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const
	{
		Vector ref_dir = -outDir + (2 * dot(outDir.normalize(), normal.normalize()) * normal);
		ref_dir = ref_dir.normalize();

		float cos_i = dot(ref_dir.normalize(), normal.normalize());
		float cos_i_sq = cos_i * cos_i;
		float two_n_cos_theta = this->two_eta * cos_i;

		float r_perp = (this->eta_kappa_sqr) - (two_n_cos_theta)+ cos_i_sq;
		r_perp = r_perp / ((this->eta_kappa_sqr) + (two_n_cos_theta)+cos_i_sq);

		float r_para = ((this->eta_kappa_sqr) * cos_i_sq) - (two_n_cos_theta) + 1;
		r_para = r_para / (((this->eta_kappa_sqr) * cos_i_sq) + (two_n_cos_theta) + 1);

		return SampleReflectance(ref_dir, RGBColor::rep(0.5f * (r_para + r_perp)) );
	}
	
	Material::Sampling MirrorMaterial::useSampling() const
	{
		return Sampling::SAMPLING_ALL;
	}

Point MirrorMaterial::getRayOrigin(Point & originalPoint) const
	{
		return originalPoint;
	}
}