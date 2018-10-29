#include "network.h"
#include "random.h"
#include <stdexcept>

void Network::resize(const size_t& n)
{
    values.clear();
    RandomNumbers num;
    for(size_t i(0); i<n; ++i)
    {
        values.push_back(num.normal());
    }
}

bool Network::add_link(const size_t& first, const size_t& second)
{
    if(first<size() and second<size() and first!=second)
    {
        auto iterator = links.find(first);
        
        if( iterator == links.end() or iterator->second != second)
        {
            links.insert({first,second});
            links.insert({second,first});
            return true;
        }
    }
    return false;
}

size_t Network::random_connect(const double& value_)
{
    size_t number_of_links(0);
    links.clear();
    for(size_t j(0);j<values.size();++j)
    {
        RandomNumbers num;
        int degree (num.poisson(value_));
        if(degree<0) degree=0;
        for(size_t i(0); i<degree ;++i)
        {
            RandomNumbers rand;
            if(add_link(j,rand.uniform_double(0,values.size()-1)))
            {
                ++number_of_links;
            }
        }
    }
    return number_of_links;
}

size_t Network::set_values(const std::vector<double>& tab_values)
{
    resize(0);
    values = tab_values;
    return values.size();
}

size_t Network::size() const
{
    return values.size();
}

size_t Network::degree(const size_t& n) const
{
    size_t degree(0);
    auto p = links.equal_range(n);
    for (auto i = p.first; i != p.second; ++i)
    {
        ++degree;
    }
    return degree;
}

double Network::value(const size_t& n) const
{
    if(n<size())
    {
        return values[n];
    }else
    {
        throw std::runtime_error("Valeur demandée inexistante");
    }
    return 0;
}

std::vector<double> Network::sorted_values() const
{
    if(size()>1)
    {
        std::vector<double> sorted_values (values);
        std::sort(sorted_values.rbegin(),sorted_values.rend());
        return sorted_values;
    } else return values;
}

std::vector<size_t> Network::neighbors(const size_t& n) const
{
    auto p = links.equal_range(n);
    std::vector<size_t> neighbors_tab;
    for (auto it(p.first); it != p.second; ++it)
    {
        neighbors_tab.push_back(it->second);
    }
    return neighbors_tab;
}
