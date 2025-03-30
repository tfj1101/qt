CREATE TABLE approvals (
    approval_id INT PRIMARY KEY AUTO_INCREMENT,
    doc_id INT NOT NULL,               -- 关联批文ID
    approver_id INT NOT NULL,          -- 审批人ID
    approval_step INT DEFAULT 1,       -- 审批步骤（如1-初审，2-复审）
    decision ENUM('approved', 'rejected', 'pending') DEFAULT 'pending', -- 审批结果
    comments TEXT,                     -- 审批意见
    approved_at DATETIME DEFAULT NOW(),
    FOREIGN KEY (doc_id) REFERENCES documents(doc_id),
    FOREIGN KEY (approver_id) REFERENCES users(user_id)
);
