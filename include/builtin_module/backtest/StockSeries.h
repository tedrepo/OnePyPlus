#include "OP_DECLARE.h"
#include "sys_module/models/SeriesBase.h"

#pragma once

OP_NAMESPACE_START

class RealizedPnlSeriesStock : public RealizedPnlSeriesBase {
  public:
    using RealizedPnlSeriesBase::RealizedPnlSeriesBase;

    void update_order(const string &ticker,
                      const double size,
                      const double execute_price,
                      const ActionType &action_type,
                      const double last_avg_price,
                      const string &long_or_short) override;
};

class CommissionSeriesStock : public CommissionSeriesBase {
  public:
    using CommissionSeriesBase::CommissionSeriesBase;
    void update_order(const string &ticker,
                      const double size,
                      const double execute_price,
                      const ActionType &action_type,
                      const double last_commission,
                      const string &long_or_short) override;
};

class HoldingPnlSeriesStock : public HoldingPnlSeriesBase {
  public:
    using HoldingPnlSeriesBase::HoldingPnlSeriesBase;
    void update_order(const string &ticker,
                      const double cur_price,
                      const double new_avg_price,
                      const double new_position,
                      const string &long_or_short) override;
    void update_barly(const bool order_executed) override;
};

class MarketValueSeriesStock : public MarketValueSeriesBase {
  public:
    using MarketValueSeriesBase::MarketValueSeriesBase;
    void update_order(const string &ticker,
                      const double cur_price,
                      const double new_position,
                      const string &long_or_short) override;
    void update_barly(const bool order_executed) override;
};

class MarginSeriesStock : public MarginSeriesBase {
  public:
    using MarginSeriesBase::MarginSeriesBase;
    void update_order(const string &ticker,
                      const string &long_or_short) override;

    void update_barly() override;
};

OP_NAMESPACE_END
