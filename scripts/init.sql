CREATE TABLE IF NOT EXISTS transactions (
    id SERIAL PRIMARY KEY,
    tx_hash VARCHAR(66) UNIQUE NOT NULL,
    from_address VARCHAR(42) NOT NULL,
    to_address VARCHAR(42),
    value_eth NUMERIC(38, 18) NOT NULL,
    block_number BIGINT NOT NULL,
    timestamp TIMESTAMP DEFAULT NOW()
);
CREATE TABLE IF NOT EXISTS meta (
    key VARCHAR(50) PRIMARY KEY,
    value VARCHAR(255)
);