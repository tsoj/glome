#pragma once

#include <iostream>
#include <limits>

namespace physics_units
{
/*
  metre     m     length
  kilogram  kg    mass
  second    s     time
  ampere    A     electric current
  kelvin    K     thermodynamic temperature
  mole      mol   amount of substance
  candela   cd    luminous intensity

  hertz 	  Hz 	  frequency 	                                                     s^(-1)
  radian 	  rad 	angle                                                            1
  steradian sr 	  solid angle                                                      1
  newton 	  N 	  force, weight 	                                                 kg*m*s^(-2)
  pascal 	  Pa 	  pressure, stress 	                                               kg*m^(-1)*s^(-2)
  joule 	  J 	  energy, work, heat 	                                             kg*m^2*s^(-2)
  watt 	    W     power, radiant flux 	                                           kg*m^2*s^(-3)
  coulomb 	C 	  electric charge or value of electricity                          s*A
  volt 	    V 	  voltage, electrical potential difference, electromotive force    kg*m^2*s^(-3)*A^(-1)
  farad 	  F 	  electrical capacitance 	                                         kg^(-1)*m^(-2)*s^4*A^2
  ohm 	    Ω 	  electrical resistance, impedance, reactance                      kg*m^2*s^(-3)*A^(-2)
  siemens 	S 	  electrical conductance 	                                         kg^(-1)*m^(-2)*s^3*A^2
  weber 	  Wb 	  magnetic flux 	                                                 kg*m^2*s^(-2)*A^(-1)
  tesla 	  T 	  magnetic induction, magnetic flux density 	                     kg*s^(-2)*A^(-1)
  henry 	  H 	  electrical inductance                                            kg*m^2*s^(-2)*A^(-2)
  lumen 	  lm 	  luminous flux 	                                                 cd
  lux 	    lx 	  illuminance 		                                                 cd*m^(-2)
  becquerel Bq 	  radioactivity (decays per unit time) 		                         s^(-1)
  gray 	    Gy 	  absorbed dose (of ionizing radiation)  	                         m^2*s^(-2)
  sievert 	Sv 	  equivalent dose (of ionizing radiation)  	                       m^2*s^(-2)
  katal 	  kat 	catalytic activity 	                                             s^(-1)*mol

  tera 	T 	1000000000000
  giga 	G 	1000000000
  mega 	M 	1000000
  kilo 	k 	1000
  hecto h 	100
  deca 	da 	10
            1
  deci 	d 	0.1
  centi c 	0.01
  milli m 	0.001
  micro μ 	0.000001
  nano 	n 	0.000000001
  pico 	p 	0.000000000001
*/

    constexpr float tera = 1'000'000'000'000;
    constexpr float giga = 1'000'000'000;
    constexpr float mega = 1'000'000;
    constexpr float kilo = 1'000;
    constexpr float hecto = 100;
    constexpr float deca = 10;
    constexpr float deci = 0.1;
    constexpr float cent = 0.01;
    constexpr float milli = 0.001;
    constexpr float micro = 0.000'001;
    constexpr float nano = 0.000'000'001;
    constexpr float pico = 0.000'000'000'001;

    namespace details
    {
        template<int m, int kg, int s, int A, int K, int mol, int cd, typename Type>
        struct Value
        {

            template<typename... Targs>
            constexpr Value(Targs&& ... args) :
                value{std::forward<Targs>(args)...}
            {}

            Type value;

            constexpr operator Type() const
            {
                return value;
            }
        };

        class EmptyClass
        {
        };

        template<typename T>
        inline constexpr auto operator*(const T& lhs, const EmptyClass&)
        {
            return lhs;
        }

        template<typename T>
        inline constexpr auto operator*(const EmptyClass&, const T& rhs)
        {
            return rhs;
        }

        template<typename T>
        inline constexpr auto operator/(const T& lhs, const EmptyClass&)
        {
            return lhs;
        }

        template<typename T>
        inline constexpr auto operator/(const EmptyClass&, const T& rhs)
        {
            return rhs;
        }

        template<int m, int kg, int s, int A, int K, int mol, int cd, typename Type>
        inline constexpr auto
        operator-(const details::Value<m, kg, s, A, K, mol, cd, Type>& rhs)
        {
            return details::Value<m, kg, s, A, K, mol, cd, decltype(-rhs.value)>{
                -rhs.value
            };
        }

        template<int m, int kg, int s, int A, int K, int mol, int cd, typename Type, typename ScalarType>
        inline constexpr auto operator*(
            const ScalarType& scalar,
            const details::Value<m, kg, s, A, K, mol, cd, Type>& rhs)
        {
            return details::Value<m, kg, s, A, K, mol, cd, decltype(scalar * rhs.value)>{
                scalar * rhs.value
            };
        }

        template<int m, int kg, int s, int A, int K, int mol, int cd, typename Type, typename ScalarType>
        inline constexpr auto operator*(
            const details::Value<m, kg, s, A, K, mol, cd, Type>& lhs,
            const ScalarType& scalar)
        {
            return details::Value<m, kg, s, A, K, mol, cd, decltype(lhs.value * scalar)>{
                lhs.value * scalar
            };
        }

        template<int m, int kg, int s, int A, int K, int mol, int cd, typename Type, typename ScalarType>
        inline constexpr auto operator/(
            const ScalarType& scalar,
            const details::Value<m, kg, s, A, K, mol, cd, Type>& rhs)
        {
            return details::Value<m, kg, s, A, K, mol, cd, decltype(scalar / rhs.value)>{
                scalar / rhs.value
            };
        }

        template<int m, int kg, int s, int A, int K, int mol, int cd, typename Type, typename ScalarType>
        inline constexpr auto operator/(
            const details::Value<m, kg, s, A, K, mol, cd, Type>& lhs,
            const ScalarType& scalar)
        {
            return details::Value<m, kg, s, A, K, mol, cd, decltype(lhs.value / scalar)>{
                lhs.value / scalar
            };
        }

        template<
            int m_l, int kg_l, int s_l, int A_l, int K_l, int mol_l, int cd_l, typename Type_l,
            int m_r, int kg_r, int s_r, int A_r, int K_r, int mol_r, int cd_r, typename Type_r
        >
        inline constexpr auto operator*(
            const details::Value<m_l, kg_l, s_l, A_l, K_l, mol_l, cd_l, Type_l>& lhs,
            const details::Value<m_r, kg_r, s_r, A_r, K_r, mol_r, cd_r, Type_r>& rhs)
        {
            return details::Value<
                m_l + m_r,
                kg_l + kg_r,
                s_l + s_r,
                A_l + A_r,
                K_l + K_r,
                mol_l + mol_r,
                cd_l + cd_r,
                decltype(lhs.value * rhs.value)>{lhs.value * rhs.value};
        }

        template<
            int m_l, int kg_l, int s_l, int A_l, int K_l, int mol_l, int cd_l, typename Type_l,
            int m_r, int kg_r, int s_r, int A_r, int K_r, int mol_r, int cd_r, typename Type_r
        >
        inline constexpr auto
        operator/(
            const details::Value<m_l, kg_l, s_l, A_l, K_l, mol_l, cd_l, Type_l>& lhs,
            const details::Value<m_r, kg_r, s_r, A_r, K_r, mol_r, cd_r, Type_r>& rhs)
        {
            return details::Value<
                m_l - m_r,
                kg_l - kg_r,
                s_l - s_r,
                A_l - A_r,
                K_l - K_r,
                mol_l - mol_r,
                cd_l - cd_r,
                decltype(lhs.value / rhs.value)>{lhs.value / rhs.value};
        }

        template<int m, int kg, int s, int A, int K, int mol, int cd, typename Type>
        inline constexpr auto
        operator+(
            const details::Value<m, kg, s, A, K, mol, cd, Type>& lhs,
            const details::Value<m, kg, s, A, K, mol, cd, Type>& rhs)
        {
            return details::Value<m, kg, s, A, K, mol, cd, decltype(lhs.value + rhs.value)>{
                lhs.value + rhs.value
            };
        }

        template<int m, int kg, int s, int A, int K, int mol, int cd, typename Type>
        inline constexpr auto
        operator-(
            const details::Value<m, kg, s, A, K, mol, cd, Type>& lhs,
            const details::Value<m, kg, s, A, K, mol, cd, Type>& rhs)
        {
            return details::Value<m, kg, s, A, K, mol, cd, decltype(lhs.value - rhs.value)>{
                lhs.value - rhs.value
            };
        }

        template<int m, int kg, int s, int A, int K, int mol, int cd, typename Type>
        inline constexpr details::Value<m, kg, s, A, K, mol, cd, Type>&
        operator+=(
            details::Value<m, kg, s, A, K, mol, cd, Type>& lhs,
            const details::Value<m, kg, s, A, K, mol, cd, Type>& rhs)
        {
            lhs.value += rhs.value;
            return lhs;
        }

        template<int m, int kg, int s, int A, int K, int mol, int cd, typename Type>
        inline constexpr details::Value<m, kg, s, A, K, mol, cd, Type>&
        operator-=(
            details::Value<m, kg, s, A, K, mol, cd, Type>& lhs,
            const details::Value<m, kg, s, A, K, mol, cd, Type>& rhs)
        {
            lhs.value -= rhs.value;
            return lhs;
        }

        template<int m, int kg, int s, int A, int K, int mol, int cd, typename Type>
        inline constexpr bool
        operator==(
            const details::Value<m, kg, s, A, K, mol, cd, Type>& lhs,
            const details::Value<m, kg, s, A, K, mol, cd, Type>& rhs)
        {
            return lhs.value == rhs.value;
        }

        template<int m, int kg, int s, int A, int K, int mol, int cd, typename Type>
        inline constexpr bool
        operator!=(
            const details::Value<m, kg, s, A, K, mol, cd, Type>& lhs,
            const details::Value<m, kg, s, A, K, mol, cd, Type>& rhs)
        {
            return lhs.value != rhs.value;
        }

        template<int m, int kg, int s, int A, int K, int mol, int cd, typename Type>
        inline constexpr bool
        operator<(
            const details::Value<m, kg, s, A, K, mol, cd, Type>& lhs,
            const details::Value<m, kg, s, A, K, mol, cd, Type>& rhs)
        {
            return lhs.value < rhs.value;
        }

        template<int m, int kg, int s, int A, int K, int mol, int cd, typename Type>
        inline constexpr bool
        operator>(
            const details::Value<m, kg, s, A, K, mol, cd, Type>& lhs,
            const details::Value<m, kg, s, A, K, mol, cd, Type>& rhs)
        {
            return lhs.value > rhs.value;
        }

        template<int m, int kg, int s, int A, int K, int mol, int cd, typename Type>
        inline constexpr bool
        operator<=(
            const details::Value<m, kg, s, A, K, mol, cd, Type>& lhs,
            const details::Value<m, kg, s, A, K, mol, cd, Type>& rhs)
        {
            return lhs.value <= rhs.value;
        }

        template<int m, int kg, int s, int A, int K, int mol, int cd, typename Type>
        inline constexpr bool
        operator>=(
            const details::Value<m, kg, s, A, K, mol, cd, Type>& lhs,
            const details::Value<m, kg, s, A, K, mol, cd, Type>& rhs)
        {
            return lhs.value >= rhs.value;
        }

    }

    template<typename Type>
    using Scalar = details::Value<0, 0, 0, 0, 0, 0, 0, Type>;

    template<typename Type> using Metre     = details::Value<1, 0, 0, 0, 0, 0, 0, Type>;
    template<typename Type> using Kilogram  = details::Value<0, 1, 0, 0, 0, 0, 0, Type>;
    template<typename Type> using Second    = details::Value<0, 0, 1, 0, 0, 0, 0, Type>;
    template<typename Type> using Ampere    = details::Value<0, 0, 0, 1, 0, 0, 0, Type>;
    template<typename Type> using Kelvin    = details::Value<0, 0, 0, 0, 1, 0, 0, Type>;
    template<typename Type> using Mole      = details::Value<0, 0, 0, 0, 0, 1, 0, Type>;
    template<typename Type> using Candela   = details::Value<0, 0, 0, 0, 0, 0, 1, Type>;

    template<typename Type> using Hertz      = details::Value<0, 0, -1, 0, 0, 0, 0, Type>;// s^(-1)
    template<typename Type> using Radian      = details::Value<0, 0, 0, 0, 0, 0, 0, Type>;// 1
    template<typename Type> using Steradian = details::Value<0, 0, 0, 0, 0, 0, 0, Type>;// 1
    template<typename Type> using Newton      = details::Value<1, 1, -2, 0, 0, 0, 0, Type>;// kg*m*s^(-2)
    template<typename Type> using Pascal      = details::Value<-1, 1, -2, 0, 0, 0, 0, Type>;// kg*m^(-1)*s^(-2)
    template<typename Type> using Joule      = details::Value<2, 1, -2, 0, 0, 0, 0, Type>;// kg*m^2*s^(-2)
    template<typename Type> using Watt        = details::Value<2, 1, -3, 0, 0, 0, 0, Type>;// kg*m^2*s^(-3)
    template<typename Type> using Coulomb    = details::Value<0, 0, 1, 1, 0, 0, 0, Type>;// s*A
    template<typename Type> using Volt        = details::Value<2, 1, -3, -1, 0, 0, 0, Type>;// kg*m^2*s^(-3)*A^(-1)
    template<typename Type> using Farad      = details::Value<-2, -1, 4, 2, 0, 0, 0, Type>;// kg^(-1)*m^(-2)*s^4*A^2
    template<typename Type> using Ohm        = details::Value<2, 1, -3, -2, 0, 0, 0, Type>;// kg*m^2*s^(-3)*A^(-2)
    template<typename Type> using Siemens    = details::Value<-2, -1, 3, 2, 0, 0, 0, Type>;// kg^(-1)*m^(-2)*s^3*A^2
    template<typename Type> using Weber      = details::Value<2, 1, -2, -1, 0, 0, 0, Type>;// kg*m^2*s^(-2)*A^(-1)
    template<typename Type> using Tesla      = details::Value<0, 1, -2, -1, 0, 0, 0, Type>;// kg*s^(-2)*A^(-1)
    template<typename Type> using Henry      = details::Value<2, 1, -2, -2, 0, 0, 0, Type>;// kg*m^2*s^(-2)*A^(-2)
    template<typename Type> using Lumen      = details::Value<0, 0, 0, 0, 0, 0, 1, Type>;// cd
    template<typename Type> using Lux        = details::Value<-2, 0, 0, 0, 0, 0, 1, Type>;// cd*m^(-2)
    template<typename Type> using Becquerel = details::Value<0, 0, -1, 0, 0, 0, 0, Type>;// s^(-1)
    template<typename Type> using Gray        = details::Value<2, 0, -2, 0, 0, 0, 0, Type>;// m^2*s^(-2)
    template<typename Type> using Sievert    = details::Value<2, 0, -2, 0, 0, 0, 0, Type>;// m^2*s^(-2)
    template<typename Type> using Katal      = details::Value<0, 0, -1, 0, 0, 1, 0, Type>;// s^(-1)*mol

    constexpr auto metre = Metre<details::EmptyClass>{};
    constexpr auto kilogram = Kilogram<details::EmptyClass>{};
    constexpr auto second = Second<details::EmptyClass>{};
    constexpr auto ampere = Ampere<details::EmptyClass>{};
    constexpr auto kelvin = Kelvin<details::EmptyClass>{};
    constexpr auto mole = Mole<details::EmptyClass>{};
    constexpr auto candela = Candela<details::EmptyClass>{};

    constexpr auto hertz = Hertz<details::EmptyClass>{};
    constexpr auto radian = Radian<details::EmptyClass>{};
    constexpr auto steradian = Steradian<details::EmptyClass>{};
    constexpr auto newton = Newton<details::EmptyClass>{};
    constexpr auto pascal = Pascal<details::EmptyClass>{};
    constexpr auto joule = Joule<details::EmptyClass>{};
    constexpr auto watt = Watt<details::EmptyClass>{};
    constexpr auto coulomb = Coulomb<details::EmptyClass>{};
    constexpr auto volt = Volt<details::EmptyClass>{};
    constexpr auto farad = Farad<details::EmptyClass>{};
    constexpr auto ohm = Ohm<details::EmptyClass>{};
    constexpr auto siemens = Siemens<details::EmptyClass>{};
    constexpr auto weber = Weber<details::EmptyClass>{};
    constexpr auto tesla = Tesla<details::EmptyClass>{};
    constexpr auto henry = Henry<details::EmptyClass>{};
    constexpr auto lumen = Lumen<details::EmptyClass>{};
    constexpr auto lux = Lux<details::EmptyClass>{};
    constexpr auto becquerel = Becquerel<details::EmptyClass>{};
    constexpr auto gray = Gray<details::EmptyClass>{};
    constexpr auto sievert = Sievert<details::EmptyClass>{};
    constexpr auto katal = Katal<details::EmptyClass>{};


} // namespace physics_units
