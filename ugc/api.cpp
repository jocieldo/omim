#include "ugc/api.hpp"

#include "platform/platform.hpp"

#include <chrono>

using namespace std;
using namespace ugc;

namespace
{
chrono::hours FromDays(uint32_t days)
{
  return std::chrono::hours(days * 24);
}
}  // namespace

namespace ugc
{
Api::Api(Index const & index, std::string const & filename) : m_index(index), m_storage(filename) {}

void Api::GetUGC(FeatureID const & id, UGCCallback callback)
{
  m_thread.Push([=] { GetUGCImpl(id, callback); });
}

void Api::GetUGCUpdate(FeatureID const & id, UGCUpdateCallback callback)
{
  m_thread.Push([=] { GetUGCUpdateImpl(id, callback); });
}

void Api::SetUGCUpdate(FeatureID const & id, UGCUpdate const & ugc)
{
  m_thread.Push([=] { SetUGCUpdate(id, ugc); });
}

// static
UGC Api::MakeTestUGC1()
{
  Rating rating;
  rating.m_ratings.emplace_back("food" /* key */, 4.0 /* value */);
  rating.m_ratings.emplace_back("service" /* key */, 5.0 /* value */);
  rating.m_ratings.emplace_back("music" /* key */, 5.0 /* value */);
  rating.m_aggValue = 4.5;

  vector<Review> reviews;
  reviews.emplace_back(20 /* id */, Text("Damn good coffee", StringUtf8Multilang::kEnglishCode),
                       Author(UID(987654321 /* hi */, 123456789 /* lo */), "Cole"),
                       5.0 /* rating */, Sentiment::Positive, Time(FromDays(10)));
  reviews.emplace_back(67812 /* id */,
                       Text("Clean place, reasonably priced", StringUtf8Multilang::kDefaultCode),
                       Author(UID(0 /* hi */, 315 /* lo */), "Cooper"), 5.0 /* rating */,
                       Sentiment::Positive, Time(FromDays(1)));

  vector<Attribute> attributes;
  attributes.emplace_back("best-drink", "Coffee");

  return UGC(rating, reviews, attributes);
}

// static
UGC Api::MakeTestUGC2()
{
  Rating rating;
  rating.m_ratings.emplace_back("food" /* key */, 5.0 /* value */);
  rating.m_ratings.emplace_back("service" /* key */, 5.0 /* value */);
  rating.m_ratings.emplace_back("music" /* key */, 5.0 /* value */);
  rating.m_aggValue = 5.0;

  vector<Review> reviews;
  reviews.emplace_back(119 /* id */,
                       Text("This pie's so good it is a crime", StringUtf8Multilang::kDefaultCode),
                       Author(UID(0 /* hi */, 315 /* lo */), "Cooper"), 5.0 /* rating */,
                       Sentiment::Positive, Time(FromDays(1)));

  vector<Attribute> attributes;
  attributes.emplace_back("best-drink", "Coffee");
  attributes.emplace_back("best-meal", "Cherry Pie");

  return UGC(rating, reviews, attributes);
}

void Api::GetUGCImpl(FeatureID const & id, UGCCallback callback)
{
  // TODO (@y, @mgsergio): retrieve static UGC
  UGC ugc(Rating({}, {}), {}, {});

  auto const r = id.m_index % 3;
  if (r == 1)
    ugc = MakeTestUGC1();
  else if (r == 2)
    ugc = MakeTestUGC2();

  GetPlatform().RunOnGuiThread([ugc, callback] { callback(ugc); });
}

void Api::GetUGCUpdateImpl(FeatureID const & /* id */, UGCUpdateCallback callback)
{
  // TODO (@y, @mgsergio): retrieve dynamic UGC
  UGCUpdate ugc(Rating({}, {}),
                Attribute({}, {}),
                ReviewAbuse({}, {}),
                ReviewFeedback({}, {}));
  GetPlatform().RunOnGuiThread([ugc, callback] { callback(ugc); });
}

void Api::SetUGCUpdateImpl(FeatureID const & id, UGCUpdate const & ugc)
{
  m_storage.SetUGCUpdate(id, ugc);
}
}  // namespace ugc